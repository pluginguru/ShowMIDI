#include "MyMidiKeyboardState.h"

MyMidiKeyboardState::MyMidiKeyboardState()
{
	zerostruct(keyDown);
	zerostruct(sustaining);
	zerostruct(pedalDown);
}

void MyMidiKeyboardState::reset()
{
	const ScopedLock sl(lock);
	zerostruct(keyDown);
	zerostruct(sustaining);
	zerostruct(pedalDown);
	eventsToAdd.clear();
}

bool MyMidiKeyboardState::isKeyDown(const int midiChannel, const int n) const noexcept
{
	int chi = midiChannel - 1;
	jassert(isPositiveAndBelow(chi, 16));

	return isPositiveAndBelow(n, 128) && keyDown[n].test(chi);
}

bool MyMidiKeyboardState::isNoteOnForChannels(const int midiChannelMask, const int n) const noexcept
{
	std::bitset<16> mask(midiChannelMask);
	return isPositiveAndBelow(n, 128) && ((keyDown[n] | sustaining[n]) & mask).any();
}

void MyMidiKeyboardState::noteOn(const int midiChannel, const int midiNoteNumber, const float velocity)
{
	jassert(midiChannel > 0 && midiChannel <= 16);
	jassert(isPositiveAndBelow(midiNoteNumber, 128));

	const ScopedLock sl(lock);

	if (isPositiveAndBelow(midiNoteNumber, 128))
	{
		int chi = midiChannel - 1;
		const int timeNow = (int)Time::getMillisecondCounter();
		if (sustaining[midiNoteNumber].test(chi))
			eventsToAdd.addEvent(MidiMessage::noteOff(midiChannel, midiNoteNumber, 0.0f), timeNow);
		eventsToAdd.addEvent(MidiMessage::noteOn(midiChannel, midiNoteNumber, velocity), timeNow);
		eventsToAdd.clear(0, timeNow - 500);

		noteOnInternal(midiChannel, midiNoteNumber, velocity);
	}
}

void MyMidiKeyboardState::noteOff(const int midiChannel, const int midiNoteNumber, const float velocity, bool force)
{
	jassert(midiChannel > 0 && midiChannel <= 16);
	jassert(isPositiveAndBelow(midiNoteNumber, 128));

	const ScopedLock sl(lock);

	if (force || isKeyDown(midiChannel, midiNoteNumber))
	{
		int chi = midiChannel - 1;
		const int timeNow = (int)Time::getMillisecondCounter();
		if (force || !sustaining[midiNoteNumber].test(chi))
		{
			eventsToAdd.addEvent(MidiMessage::noteOff(midiChannel, midiNoteNumber), timeNow);
			eventsToAdd.clear(0, timeNow - 500);
		}

		noteOffInternal(midiChannel, midiNoteNumber, velocity);
	}
}

void MyMidiKeyboardState::noteOnInternal(const int midiChannel, const int midiNoteNumber, const float velocity)
{
	if (isPositiveAndBelow(midiNoteNumber, 128))
	{
		int chi = midiChannel - 1;
		keyDown[midiNoteNumber].set(chi);
		if (pedalDown.test(chi)) sustaining[midiNoteNumber].set(chi);
		listeners.call([&](Listener& l) { l.handleNoteOn(this, midiChannel, midiNoteNumber, velocity); });
	}
}

void MyMidiKeyboardState::noteOffInternal(const int midiChannel, const int midiNoteNumber, const float velocity)
{
	if (isKeyDown(midiChannel, midiNoteNumber))
	{
		keyDown[midiNoteNumber].reset(midiChannel - 1);
		listeners.call([&](Listener& l) { l.handleNoteOff(this, midiChannel, midiNoteNumber, velocity); });
	}
}

void MyMidiKeyboardState::allNotesOff(const int midiChannel)
{
	const ScopedLock sl(lock);

	if (midiChannel <= 0)
	{
		for (int i = 1; i <= 16; ++i)
			allNotesOff(i);
	}
	else
	{
		for (int i = 0; i < 128; ++i)
			noteOff(midiChannel, i, 0.0f);
	}
}

void MyMidiKeyboardState::pedal(int midiChannel, bool down)
{
	int chi = midiChannel - 1;
	jassert(isPositiveAndBelow(chi, 16));

	bool wasDown = pedalDown.test(chi);
	pedalDown.set(chi, down);

	if (!wasDown && down)
	{
		// pedal just went down: all notes which are down are now also sustaining
		for (int i = 0; i < 128; ++i)
			if (keyDown[i].test(chi)) sustaining[i].set(chi);
		// alert listeners
		listeners.call([&](Listener& l) { l.handlePedalDown(this, midiChannel); });
	}
	else if (wasDown && !down)
	{
		// pedal just came up: turn off all sustaining notes which are not also down
		for (int i = 0; i < 128; ++i)
			if (sustaining[i].test(chi) && !(keyDown[i].test(chi)))
				noteOff(midiChannel, i, 0.0f, true);
		// clear all sustaining notes
		for (int i = 0; i < 128; ++i)
			sustaining[i].set(chi, false);
		// alert listeners
		listeners.call([&](Listener& l) { l.handlePedalUp(this, midiChannel); });
	}
}

void MyMidiKeyboardState::processNextMidiEvent(const MidiMessage& message)
{
	if (message.isNoteOn())
	{
		noteOnInternal(message.getChannel(), message.getNoteNumber(), message.getFloatVelocity());
	}
	else if (message.isNoteOff())
	{
		noteOffInternal(message.getChannel(), message.getNoteNumber(), message.getFloatVelocity());
	}
	else if (message.isSustainPedalOn())
	{
		pedal(message.getChannel(), true);
	}
	else if (message.isSustainPedalOff())
	{
		pedal(message.getChannel(), false);
	}
	else if (message.isAllNotesOff())
	{
		for (int i = 0; i < 128; ++i)
			noteOffInternal(message.getChannel(), i, 0.0f);
	}
}

void MyMidiKeyboardState::processNextMidiBuffer(MidiBuffer& buffer,
	const int startSample,
	const int numSamples,
	const bool injectIndirectEvents)
{
	const ScopedLock sl(lock);

	for (const auto metadata : buffer)
		processNextMidiEvent(metadata.getMessage());

	if (injectIndirectEvents)
	{
		const int firstEventToAdd = eventsToAdd.getFirstEventTime();
		const double scaleFactor = numSamples / (double)(eventsToAdd.getLastEventTime() + 1 - firstEventToAdd);

		for (const auto metadata : eventsToAdd)
		{
			const auto pos = jlimit(0, numSamples - 1, roundToInt((metadata.samplePosition - firstEventToAdd) * scaleFactor));
			buffer.addEvent(metadata.getMessage(), startSample + pos);
		}
	}

	eventsToAdd.clear();
}

void MyMidiKeyboardState::addListener(Listener* listener)
{
	const ScopedLock sl(lock);
	listeners.add(listener);
}

void MyMidiKeyboardState::removeListener(Listener* listener)
{
	const ScopedLock sl(lock);
	listeners.remove(listener);
}

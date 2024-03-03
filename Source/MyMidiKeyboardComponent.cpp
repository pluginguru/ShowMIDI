#include "MyMidiKeyboardComponent.h"

MyMidiKeyboardComponent::MyMidiKeyboardComponent (MyMidiKeyboardState& stateToUse, Orientation orientationToUse)
    : KeyboardComponentBase (orientationToUse), state (stateToUse)
{
    state.addListener (this);

    // initialise with a default set of qwerty key-mappings.
    const std::string_view keys { "awsedftgyhujkolp;" };

    for (const char& c : keys)
        setKeyPressForNote ({c, 0, 0}, (int) std::distance (keys.data(), &c));

    mouseOverNotes.insertMultiple (0, -1, 32);
    mouseDownNotes.insertMultiple (0, -1, 32);

    colourChanged();
    setWantsKeyboardFocus (true);

    // SD add
    pedalUp();

    startTimerHz (20);
}

MyMidiKeyboardComponent::~MyMidiKeyboardComponent()
{
    state.removeListener (this);
}

void MyMidiKeyboardComponent::setVelocity (float v, bool useMousePosition)
{
    velocity = v;
    useMousePositionForVelocity = useMousePosition;
}

void MyMidiKeyboardComponent::setMidiChannel (int midiChannelNumber)
{
    jassert (midiChannelNumber > 0 && midiChannelNumber <= 16);

    if (midiChannel != midiChannelNumber)
    {
        resetAnyKeysInUse();
        midiChannel = jlimit (1, 16, midiChannelNumber);
    }
}

void MyMidiKeyboardComponent::setMidiChannelsToDisplay (int midiChannelMask)
{
    midiInChannelMask = midiChannelMask;
    noPendingUpdates.store (false);
}

void MyMidiKeyboardComponent::clearKeyMappings()
{
    resetAnyKeysInUse();
    keyPressNotes.clear();
    keyPresses.clear();
}

void MyMidiKeyboardComponent::setKeyPressForNote (const KeyPress& key, int midiNoteOffsetFromC)
{
    removeKeyPressForNote (midiNoteOffsetFromC);

    keyPressNotes.add (midiNoteOffsetFromC);
    keyPresses.add (key);
}

void MyMidiKeyboardComponent::removeKeyPressForNote (int midiNoteOffsetFromC)
{
    for (int i = keyPressNotes.size(); --i >= 0;)
    {
        if (keyPressNotes.getUnchecked (i) == midiNoteOffsetFromC)
        {
            keyPressNotes.remove (i);
            keyPresses.remove (i);
        }
    }
}

void MyMidiKeyboardComponent::setKeyPressBaseOctave (int newOctaveNumber)
{
    jassert (newOctaveNumber >= 0 && newOctaveNumber <= 10);

    keyMappingOctave = newOctaveNumber;
}

void MyMidiKeyboardComponent::resetAnyKeysInUse()
{
    if (! keysPressed.isZero())
    {
        for (int i = 128; --i >= 0;)
            if (keysPressed[i])
                state.noteOff (midiChannel, i, 0.0f);

        keysPressed.clear();
    }

    for (int i = mouseDownNotes.size(); --i >= 0;)
    {
        auto noteDown = mouseDownNotes.getUnchecked (i);

        if (noteDown >= 0)
        {
            state.noteOff (midiChannel, noteDown, 0.0f);
            mouseDownNotes.set (i, -1);
        }

        mouseOverNotes.set (i, -1);
    }
}

void MyMidiKeyboardComponent::updateNoteUnderMouse (const MouseEvent& e, bool isDown)
{
    updateNoteUnderMouse (e.getEventRelativeTo (this).position, isDown, e.source.getIndex());
}

void MyMidiKeyboardComponent::updateNoteUnderMouse (Point<float> pos, bool isDown, int fingerNum)
{
    const auto noteInfo = getNoteAndVelocityAtPosition (pos);
    const auto newNote = noteInfo.note;
    const auto oldNote = mouseOverNotes.getUnchecked (fingerNum);
    const auto oldNoteDown = mouseDownNotes.getUnchecked (fingerNum);
    const auto eventVelocity = useMousePositionForVelocity ? noteInfo.velocity * velocity : velocity;

    if (oldNote != newNote)
    {
        repaintNote (oldNote);
        repaintNote (newNote);
        mouseOverNotes.set (fingerNum, newNote);
    }

    if (isDown)
    {
        if (newNote != oldNoteDown)
        {
            if (oldNoteDown >= 0)
            {
                mouseDownNotes.set (fingerNum, -1);

                if (! mouseDownNotes.contains (oldNoteDown))
                    state.noteOff (midiChannel, oldNoteDown, eventVelocity);
            }

            if (newNote >= 0 && ! mouseDownNotes.contains (newNote))
            {
                state.noteOn (midiChannel, newNote, eventVelocity);
                mouseDownNotes.set (fingerNum, newNote);
            }
        }
    }
    else if (oldNoteDown >= 0)
    {
        mouseDownNotes.set (fingerNum, -1);

        if (! mouseDownNotes.contains (oldNoteDown))
            state.noteOff (midiChannel, oldNoteDown, eventVelocity);
    }
}

void MyMidiKeyboardComponent::repaintNote (int noteNum)
{
    if (getRangeStart() <= noteNum && noteNum <= getRangeEnd())
        repaint (getRectangleForKey (noteNum).getSmallestIntegerContainer());
}


void MyMidiKeyboardComponent::mouseMove (const MouseEvent& e)
{
    updateNoteUnderMouse (e, false);
}

void MyMidiKeyboardComponent::mouseDrag (const MouseEvent& e)
{
    auto newNote = getNoteAndVelocityAtPosition (e.position).note;

    if (newNote >= 0 && mouseDraggedToKey (newNote, e))
        updateNoteUnderMouse (e, true);
}

void MyMidiKeyboardComponent::mouseDown (const MouseEvent& e)
{
    auto newNote = getNoteAndVelocityAtPosition (e.position).note;

    if (newNote >= 0 && mouseDownOnKey (newNote, e))
        updateNoteUnderMouse (e, true);
}

void MyMidiKeyboardComponent::mouseUp (const MouseEvent& e)
{
    updateNoteUnderMouse (e, false);

    auto note = getNoteAndVelocityAtPosition (e.position).note;

    if (note >= 0)
        mouseUpOnKey (note, e);
}

void MyMidiKeyboardComponent::mouseEnter (const MouseEvent& e)
{
    updateNoteUnderMouse (e, false);
}

void MyMidiKeyboardComponent::mouseExit (const MouseEvent& e)
{
    updateNoteUnderMouse (e, false);
}

void MyMidiKeyboardComponent::timerCallback()
{
    if (noPendingUpdates.exchange (true))
        return;

    for (auto i = getRangeStart(); i <= getRangeEnd(); ++i)
    {
        const auto isOn = state.isNoteOnForChannels (midiInChannelMask, i);

        if (keysCurrentlyDrawnDown[i] != isOn)
        {
            keysCurrentlyDrawnDown.setBit (i, isOn);
            repaintNote (i);
        }
    }
}

bool MyMidiKeyboardComponent::keyStateChanged (bool /*isKeyDown*/)
{
    bool keyPressUsed = false;

    for (int i = keyPresses.size(); --i >= 0;)
    {
        auto note = 12 * keyMappingOctave + keyPressNotes.getUnchecked (i);

        if (keyPresses.getReference (i).isCurrentlyDown())
        {
            if (! keysPressed[note])
            {
                keysPressed.setBit (note);
                state.noteOn (midiChannel, note, velocity);
                keyPressUsed = true;
            }
        }
        else
        {
            if (keysPressed[note])
            {
                keysPressed.clearBit (note);
                state.noteOff (midiChannel, note, 0.0f);
                keyPressUsed = true;
            }
        }
    }

    return keyPressUsed;
}

bool MyMidiKeyboardComponent::keyPressed (const KeyPress& key)
{
    return keyPresses.contains (key);
}

void MyMidiKeyboardComponent::focusLost (FocusChangeType)
{
    resetAnyKeysInUse();
}

void MyMidiKeyboardComponent::drawKeyboardBackground (Graphics& g, Rectangle<float> area)
{
    g.fillAll (findColour (whiteNoteColourId));

    auto width = area.getWidth();
    auto height = area.getHeight();
    auto currentOrientation = getOrientation();
    Point<float> shadowGradientStart, shadowGradientEnd;

    if (currentOrientation == verticalKeyboardFacingLeft)
    {
        shadowGradientStart.x = width - 1.0f;
        shadowGradientEnd.x   = width - 5.0f;
    }
    else if (currentOrientation == verticalKeyboardFacingRight)
    {
        shadowGradientEnd.x = 5.0f;
    }
    else
    {
        shadowGradientEnd.y = 5.0f;
    }

    auto keyboardWidth = getRectangleForKey (getRangeEnd()).getRight();
    auto shadowColour = findColour (shadowColourId);

    if (! shadowColour.isTransparent())
    {
        g.setGradientFill ({ shadowColour, shadowGradientStart,
                             shadowColour.withAlpha (0.0f), shadowGradientEnd,
                             false });

        switch (currentOrientation)
        {
            case horizontalKeyboard:            g.fillRect (0.0f, 0.0f, keyboardWidth, 5.0f); break;
            case verticalKeyboardFacingLeft:    g.fillRect (width - 5.0f, 0.0f, 5.0f, keyboardWidth); break;
            case verticalKeyboardFacingRight:   g.fillRect (0.0f, 0.0f, 5.0f, keyboardWidth); break;
            default: break;
        }
    }

    auto lineColour = findColour (keySeparatorLineColourId);

    if (! lineColour.isTransparent())
    {
        g.setColour (lineColour);

        switch (currentOrientation)
        {
            case horizontalKeyboard:            g.fillRect (0.0f, height - 1.0f, keyboardWidth, 1.0f); break;
            case verticalKeyboardFacingLeft:    g.fillRect (0.0f, 0.0f, 1.0f, keyboardWidth); break;
            case verticalKeyboardFacingRight:   g.fillRect (width - 1.0f, 0.0f, 1.0f, keyboardWidth); break;
            default: break;
        }
    }
}

void MyMidiKeyboardComponent::drawWhiteNote (int midiNoteNumber, Graphics& g, Rectangle<float> area,
                                             bool isDown, bool isOver, Colour lineColour, Colour textColour)
{
    auto c = Colours::transparentWhite;

    if (isDown)  c = findColour (keyDownOverlayColourId);
    if (isOver)  c = c.overlaidWith (findColour (mouseOverKeyOverlayColourId));

    g.setColour (c);
    g.fillRect (area);

    const auto currentOrientation = getOrientation();

    auto text = getWhiteNoteText (midiNoteNumber);

    if (text.isNotEmpty())
    {
        auto fontHeight = jmin (12.0f, getKeyWidth() * 0.9f);

        g.setColour (textColour);
        g.setFont (Font (fontHeight).withHorizontalScale (0.8f));

        switch (currentOrientation)
        {
            case horizontalKeyboard:            g.drawText (text, area.withTrimmedLeft (1.0f).withTrimmedBottom (2.0f), Justification::centredBottom, false); break;
            case verticalKeyboardFacingLeft:    g.drawText (text, area.reduced (2.0f), Justification::centredLeft,   false); break;
            case verticalKeyboardFacingRight:   g.drawText (text, area.reduced (2.0f), Justification::centredRight,  false); break;
            default: break;
        }
    }

    if (! lineColour.isTransparent())
    {
        g.setColour (lineColour);

        switch (currentOrientation)
        {
            case horizontalKeyboard:            g.fillRect (area.withWidth (1.0f)); break;
            case verticalKeyboardFacingLeft:    g.fillRect (area.withHeight (1.0f)); break;
            case verticalKeyboardFacingRight:   g.fillRect (area.removeFromBottom (1.0f)); break;
            default: break;
        }

        if (midiNoteNumber == getRangeEnd())
        {
            switch (currentOrientation)
            {
                case horizontalKeyboard:            g.fillRect (area.expanded (1.0f, 0).removeFromRight (1.0f)); break;
                case verticalKeyboardFacingLeft:    g.fillRect (area.expanded (0, 1.0f).removeFromBottom (1.0f)); break;
                case verticalKeyboardFacingRight:   g.fillRect (area.expanded (0, 1.0f).removeFromTop (1.0f)); break;
                default: break;
            }
        }
    }
}

void MyMidiKeyboardComponent::drawBlackNote (int /*midiNoteNumber*/, Graphics& g, Rectangle<float> area,
                                           bool isDown, bool isOver, Colour noteFillColour)
{
    auto c = noteFillColour;

    if (isDown)  c = c.overlaidWith (findColour (keyDownOverlayColourId));
    if (isOver)  c = c.overlaidWith (findColour (mouseOverKeyOverlayColourId));

    g.setColour (c);
    g.fillRect (area);

    if (isDown)
    {
        g.setColour (noteFillColour);
        g.drawRect (area);
    }
    else
    {
        g.setColour (c.brighter());
        auto sideIndent = 1.0f / 8.0f;
        auto topIndent = 7.0f / 8.0f;
        auto w = area.getWidth();
        auto h = area.getHeight();

        switch (getOrientation())
        {
            case horizontalKeyboard:            g.fillRect (area.reduced (w * sideIndent, 0).removeFromTop   (h * topIndent)); break;
            case verticalKeyboardFacingLeft:    g.fillRect (area.reduced (0, h * sideIndent).removeFromRight (w * topIndent)); break;
            case verticalKeyboardFacingRight:   g.fillRect (area.reduced (0, h * sideIndent).removeFromLeft  (w * topIndent)); break;
            default: break;
        }
    }
}

String MyMidiKeyboardComponent::getWhiteNoteText (int midiNoteNumber)
{
    if (midiNoteNumber % 12 == 0)
        return MidiMessage::getMidiNoteName (midiNoteNumber, true, true, getOctaveForMiddleC());

    return {};
}

void MyMidiKeyboardComponent::colourChanged()
{
    setOpaque (findColour (whiteNoteColourId).isOpaque());
    repaint();
}

void MyMidiKeyboardComponent::drawWhiteKey (int midiNoteNumber, Graphics& g, Rectangle<float> area)
{
    drawWhiteNote (midiNoteNumber, g, area, state.isNoteOnForChannels (midiInChannelMask, midiNoteNumber),
                   mouseOverNotes.contains (midiNoteNumber), findColour (keySeparatorLineColourId), findColour (textLabelColourId));
}

void MyMidiKeyboardComponent::drawBlackKey (int midiNoteNumber, Graphics& g, Rectangle<float> area)
{
    drawBlackNote (midiNoteNumber, g, area, state.isNoteOnForChannels (midiInChannelMask, midiNoteNumber),
                   mouseOverNotes.contains (midiNoteNumber), findColour (blackNoteColourId));
}

void MyMidiKeyboardComponent::handleNoteOn (MyMidiKeyboardState*, int /*midiChannel*/, int /*midiNoteNumber*/, float /*velocity*/)
{
    noPendingUpdates.store (false);
}

void MyMidiKeyboardComponent::handleNoteOff (MyMidiKeyboardState*, int /*midiChannel*/, int /*midiNoteNumber*/, float /*velocity*/)
{
    noPendingUpdates.store (false);
}

void MyMidiKeyboardComponent::handlePedalDown(MyMidiKeyboardState*, int /*midiChannel*/)
{
    noPendingUpdates.store(false);
}

void MyMidiKeyboardComponent::handlePedalUp(MyMidiKeyboardState*, int /*midiChannel*/)
{
    noPendingUpdates.store(false);
}

bool MyMidiKeyboardComponent::mouseDownOnKey    ([[maybe_unused]] int midiNoteNumber, [[maybe_unused]] const MouseEvent& e)  { return true; }
bool MyMidiKeyboardComponent::mouseDraggedToKey ([[maybe_unused]] int midiNoteNumber, [[maybe_unused]] const MouseEvent& e)  { return true; }
void MyMidiKeyboardComponent::mouseUpOnKey      ([[maybe_unused]] int midiNoteNumber, [[maybe_unused]] const MouseEvent& e)  {}

#include "SustainPedalLogic.h"

SustainPedalLogic::SustainPedalLogic()
{
    clear();
}

void SustainPedalLogic::clear()
{
    for (int i = 0; i < kMidiNoteNumbers; i++)
    {
        keyDown.reset(i);
        isPlaying.reset(i);
    }
    pedalIsDown = false;
}

void SustainPedalLogic::clearSustainingNotes()
{
    for (int i = 0; i < kMidiNoteNumbers; i++)
        if (isPlaying.test(i) && !keyDown.test(i)) isPlaying.reset(i);
}

bool SustainPedalLogic::keyDownAction(int noteNumber)
{
    bool noteShouldStopBeforePlayingAgain = false;
    
    if (pedalIsDown && isPlaying.test(noteNumber))
        noteShouldStopBeforePlayingAgain = true;
    
    keyDown.set(noteNumber);
    isPlaying.set(noteNumber);
    return noteShouldStopBeforePlayingAgain;
}

bool SustainPedalLogic::keyUpAction(int noteNumber)
{
    bool noteShouldStop = false;
    
    if (!pedalIsDown)
    {
        noteShouldStop = true;
        isPlaying.reset(noteNumber);
    }
    keyDown.reset(noteNumber);
    return noteShouldStop;
}

bool SustainPedalLogic::isNoteSustaining(int noteNumber)
{
    return isPlaying.test(noteNumber) && !keyDown.test(noteNumber);
}

bool SustainPedalLogic::isAnyNotePlaying(int lokey, int hikey)
{
    for (int i = lokey; i <= hikey; i++)
        if (isPlaying.test(i)) return true;
    return false;
}

bool SustainPedalLogic::isAnyNoteSustaining(int lokey, int hikey)
{
    for (int i = lokey; i <= hikey; i++)
        if (isPlaying.test(i) && !keyDown.test(i)) return true;
    return false;
}

bool SustainPedalLogic::isAnyKeyDown(int lokey, int hikey)
{
    for (int i = lokey; i <= hikey; i++) {
        if (keyDown[i] != 0) {
            return true;
        }
    }
    return false;
}

bool SustainPedalLogic::getAllNotesDown(Array<int>& outNotes)
{
    outNotes.clear();
    for (int i = 0; i < kMidiNoteNumbers; i++) if (keyDown.test(i)) outNotes.add(i);
    return !outNotes.isEmpty();
}

bool SustainPedalLogic::getAllNotesPlaying(Array<int>& outNotes)
{
    outNotes.clear();
    for (int i = 0; i < kMidiNoteNumbers; i++) if (isPlaying.test(i)) outNotes.add(i);
    return !outNotes.isEmpty();
}


bool MultiChannelSustainPedalLogic::isPedalDown()
{
    for (int chIdx = 0; chIdx < kMidiChannels; chIdx++)
        if (logic[chIdx].isPedalDown()) return true;
    return false;
}

bool MultiChannelSustainPedalLogic::isAnyKeyDown(int lokey, int hikey)
{
    for (int chIdx = 0; chIdx < kMidiChannels; chIdx++)
        if (logic[chIdx].isAnyKeyDown(lokey, hikey)) return true;
    return false;
}

bool MultiChannelSustainPedalLogic::isAnyNoteSustaining(int lokey, int hikey)
{
    for (int chIdx = 0; chIdx < kMidiChannels; chIdx++)
        if (logic[chIdx].isAnyNoteSustaining(lokey, hikey)) return true;
    return false;
}

#pragma once
#include <JuceHeader.h>
#include <bitset>

static const size_t kMidiNoteNumbers = 128;

class SustainPedalLogic
{
    std::bitset<kMidiNoteNumbers> keyDown;
    std::bitset<kMidiNoteNumbers> isPlaying;
    bool pedalIsDown;
    
public:
    SustainPedalLogic();
    
    // return true if given note should stop playing
    bool keyDownAction(int noteNumber);
    bool keyUpAction(int noteNumber);
    
    // change status
    void clear();
    void setPedalState(bool down) { pedalIsDown = down; }
    void pedalDown() { setPedalState(true); }
    void pedalUp() { setPedalState(false); }
    void clearSustainingNote(int nn) { isPlaying.reset(nn); }
    void clearSustainingNotes();

    // status checks
    bool isKeyDown(int nn) { return keyDown.test(nn); }
    bool isPedalDown() { return pedalIsDown; }
    bool isNotePlaying(int nn) { return isPlaying.test(nn); }
    bool isAnyNotePlaying(int lokey = 0, int hikey = 127);
    bool isNoteSustaining(int);
    bool isAnyNoteSustaining(int lokey = 0, int hikey = 127);
    bool isAnyKeyDown(int lokey=0, int hikey=127);
    bool getAllNotesDown(Array<int>&);
    bool getAllNotesPlaying(Array<int>&);
};


static const int kMidiChannels = 16;

class MultiChannelSustainPedalLogic
{
    SustainPedalLogic logic[kMidiChannels];

public:
    // return true if given note should stop playing
    bool keyDownAction(int chIdx, int noteNumber) { return logic[chIdx].keyDownAction(noteNumber); }
    bool keyUpAction(int chIdx, int noteNumber) { return logic[chIdx].keyUpAction(noteNumber); }

    // change status
    void clear() { for (int chIdx = 0; chIdx < kMidiChannels; chIdx++) logic[chIdx].clear(); }
    void setPedalState(int chIdx, bool down) { logic[chIdx].setPedalState(down); }
    void pedalDown(int chIdx) { logic[chIdx].pedalDown(); }
    void pedalUp(int chIdx) { logic[chIdx].pedalUp(); }
    void clearSustainingNotes(int chIdx) { logic[chIdx].clearSustainingNotes(); }

    // status checks
    bool isPedalDown();
    bool isAnyKeyDown(int lokey = 0, int hikey = 127);
    bool isAnyNoteSustaining(int lokey = 0, int hikey = 127);
};

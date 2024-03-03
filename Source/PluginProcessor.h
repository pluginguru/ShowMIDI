#pragma once
#include <JuceHeader.h>
#include "MyMidiKeyboardState.h"

struct MidiMessageFifo
{
    void add(MidiMessage msg)
    {
        int start1, size1, start2, size2;
        abstractFifo.prepareToWrite(1, start1, size1, start2, size2);
        if (size1 > 0) msgBuf[start1] = msg;
        if (size2 > 0) msgBuf[start2] = msg;
        abstractFifo.finishedWrite(size1 + size2);
    }

    int getNumReady()
    {
        return abstractFifo.getNumReady();
    }

    void read(MidiMessage& msg)
    {
        int start1, size1, start2, size2;
        abstractFifo.prepareToRead(1, start1, size1, start2, size2);
        if (size1 > 0) msg = msgBuf[start1];
        if (size2 > 0) msg = msgBuf[start2];
        abstractFifo.finishedRead(size1 + size2);
    }

    AbstractFifo abstractFifo{ 100 };
    MidiMessage msgBuf[100];
};

class ShowMidiProcessor : public AudioProcessor
                        , public ChangeBroadcaster
{
public:
    ShowMidiProcessor();

    // AudioProcessor
    bool isMidiEffect() const override { return true; }
    const String getName() const override { return "ShowMIDI"; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return true; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const String getProgramName (int) override { return {}; }
    void changeProgramName (int, const String&) override {}

    bool hasEditor() const override { return true; }
    AudioProcessorEditor* createEditor() override;

    void prepareToPlay(double, int) override {}
    void releaseResources() override {}
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // ShowMidiProcessor
    bool isVST() const;

    int keyCount;       // number of keys displayed on keyboard
    int ccCount;        // number of CC indicators (1st one is pitch bend)
    int lastUIWidth, lastUIHeight;
    int cc1, cc2, cc3, cc4;

    float pitchBend;    // range -1.0 to +1.0, normal value 0.0
    float cc1Value, cc2Value, cc3Value, cc4Value;     // range 0.0 to 1.0
    bool sustainPedalDown;

protected:
    MyMidiKeyboardState keyboardState;
    MidiMessageFifo midiFifo;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ShowMidiProcessor)
};

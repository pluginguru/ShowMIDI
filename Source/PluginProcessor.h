#pragma once
#include <JuceHeader.h>
#include "MyMidiKeyboardState.h"

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
    float modWheel;     // range 0.0 to 1.0
    float breathController, footController, softPedal;
    bool sustainPedalDown;

protected:
    MyMidiKeyboardState keyboardState;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ShowMidiProcessor)
};

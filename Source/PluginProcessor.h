/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SustainPedalLogic.hpp"

//==============================================================================
/**
*/
class KeyboardPluginAudioProcessor  : public AudioProcessor
                                    , public ChangeBroadcaster
{
public:
    //==============================================================================
    KeyboardPluginAudioProcessor();
    ~KeyboardPluginAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    bool isVST() const;

    //==============================================================================
    bool isMidiEffect() const override                     { return true; }

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override                  { return "Keyboard"; }

    bool acceptsMidi() const override                      { return true; }
    bool producesMidi() const override                     { return true; }
    double getTailLengthSeconds() const override           { return 0; }

    //==============================================================================
    int getNumPrograms() override                          { return 1; }
    int getCurrentProgram() override                       { return 0; }
    void setCurrentProgram (int) override                  {}
    const String getProgramName (int) override             { return {}; }
    void changeProgramName (int, const String&) override   {}

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    int keyCount;       // number of keys displayed on keyboard
    int ccCount;        // number of CC indicators (1st one is pitch bend)
    int lastUIWidth, lastUIHeight;
    int cc1, cc2, cc3, cc4;

    float pitchBend;    // range -1.0 to +1.0, normal value 0.0
    float modWheel;     // range 0.0 to 1.0
    float breathController, footController, softPedal;
    bool sustainPedalDown;

private:
    MidiKeyboardState keyboardState;
    AudioKitCore::SustainPedalLogic pedalLogic;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyboardPluginAudioProcessor)
};

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

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    bool isMidiEffect() const override                     { return false; }

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

    int keyCount;
    int lastUIWidth, lastUIHeight;

    float pitchBend;    // range -1.0 to +1.0, normal value 0.0
    float modWheel;     // range 0.0 to 1.0
    bool sustainPedalDown;

private:
    MidiKeyboardState keyboardState;
    AudioKitCore::SustainPedalLogic pedalLogic;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyboardPluginAudioProcessor)
};

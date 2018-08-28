#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "UnidirectionalSlider.h"
#include "BidirectionalSlider.h"

class KeyboardPluginAudioProcessorEditor    : public AudioProcessorEditor
                                            , public ChangeListener
{
public:
    KeyboardPluginAudioProcessorEditor (KeyboardPluginAudioProcessor&, MidiKeyboardState&);
    ~KeyboardPluginAudioProcessorEditor();

    // Component
    void paint (Graphics&) override;
    void resized() override;

    // ChangeListener
    void changeListenerCallback(ChangeBroadcaster*) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    KeyboardPluginAudioProcessor& processor;
    
    MidiKeyboardComponent keyboardComponent;
    BidirectionalSlider pitchWheel;
    UnidirectionalSlider modWheel;

    ComponentBoundsConstrainer resizeLimits;
    std::unique_ptr<ResizableCornerComponent> resizer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyboardPluginAudioProcessorEditor)
};

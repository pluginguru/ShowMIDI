#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "UnidirectionalSlider.h"
#include "BidirectionalSlider.h"
#include "BooleanIndicator.h"
#include "MyMidiKeyboardComponent.h"

class KeyboardPluginAudioProcessorEditor    : public AudioProcessorEditor
                                            , public ChangeListener
{
public:
    KeyboardPluginAudioProcessorEditor (KeyboardPluginAudioProcessor&, MidiKeyboardState&);
    ~KeyboardPluginAudioProcessorEditor();

    // Component
    void paint (Graphics&) override;
    void resized() override;
    void mouseDown(const MouseEvent&) override;

    // ChangeListener
    void changeListenerCallback(ChangeBroadcaster*) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    KeyboardPluginAudioProcessor& processor;
    
    MyMidiKeyboardComponent keyboardComponent;
    BooleanIndicator sustainPedal;
    BidirectionalSlider pitchWheel;
    UnidirectionalSlider modWheel;
    UnidirectionalSlider breathController, footController, softPedal;

    TextButton keyboardButton, ccButton;

    ComponentBoundsConstrainer resizeLimits;
    std::unique_ptr<ResizableCornerComponent> resizer;

    bool pedalDown;
    int ccCount;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyboardPluginAudioProcessorEditor)
};

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UnidirectionalSlider.h"
#include "BidirectionalSlider.h"
#include "BooleanIndicator.h"
#include "MyMidiKeyboardComponent.h"

class ShowMidiEditor    : public AudioProcessorEditor
                        , public ChangeListener
{
public:
    ShowMidiEditor (ShowMidiProcessor&, MidiKeyboardState&);
    ~ShowMidiEditor();

    // Component
    void paint (Graphics&) override;
    void resized() override;

    // ChangeListener
    void changeListenerCallback(ChangeBroadcaster*) override;

private:
    ShowMidiProcessor& processor;
    
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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ShowMidiEditor)
};

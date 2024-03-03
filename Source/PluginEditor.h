#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MidiCcSlider.h"
#include "MidiPitchSlider.h"
#include "BooleanIndicator.h"
#include "MyMidiKeyboardComponent.h"

class MyMidiKeyboardState;

class ShowMidiEditor    : public AudioProcessorEditor
                        , public ChangeListener
{
public:
    ShowMidiEditor (ShowMidiProcessor&, MyMidiKeyboardState&, MidiMessageFifo&);
    ~ShowMidiEditor();

    // Component
    void paint (Graphics&) override;
    void resized() override;

    // ChangeListener
    void changeListenerCallback(ChangeBroadcaster*) override;

private:
    ShowMidiProcessor& processor;
    MidiMessageFifo& midiFifo;
    
    MyMidiKeyboardComponent keyboardComponent;
    TogglingBooleanIndicator pedalIndicator;
    MidiPitchSlider pitchSlider;
    MidiCcSlider ccSlider1, ccSlider2, ccSlider3, ccSlider4;

    TextButton keyboardButton, ccButton;

    ComponentBoundsConstrainer resizeLimits;
    std::unique_ptr<ResizableCornerComponent> resizer;

    bool pedalDown;
    int ccCount;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ShowMidiEditor)
};

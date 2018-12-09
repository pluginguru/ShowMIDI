#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class MyMidiKeyboardComponent : public MidiKeyboardComponent
{
public:
    MyMidiKeyboardComponent (MidiKeyboardState& state, Orientation orientation)
    : MidiKeyboardComponent(state, orientation) {
        setColour(MidiKeyboardComponent::keyDownOverlayColourId, Colours::mediumturquoise);
    }
    
    void mouseMove (const MouseEvent &) override {}
    void mouseDrag (const MouseEvent &) override {}
    void mouseDown (const MouseEvent &) override {}
    void mouseUp (const MouseEvent &) override {}
    void mouseEnter (const MouseEvent &) override {}
    void mouseExit (const MouseEvent &) override {}
    void mouseWheelMove (const MouseEvent &, const MouseWheelDetails &) override {}

    void pedalDown() {
        setColour(MidiKeyboardComponent::keyDownOverlayColourId, Colours::violet);
    }
    void pedalUp() {
        setColour(MidiKeyboardComponent::keyDownOverlayColourId, Colours::mediumturquoise);
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyMidiKeyboardComponent)
};

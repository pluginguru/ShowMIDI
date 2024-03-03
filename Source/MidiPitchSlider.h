#pragma once
#include <JuceHeader.h>

class MidiPitchSlider	: public Component
{
public:
    MidiPitchSlider() : value(0.0f) {}
    virtual ~MidiPitchSlider() = default;
    
	// Component
	void paint(Graphics&) override;
    void mouseDown(const MouseEvent&) override;
    void mouseDrag(const MouseEvent&) override;
    void mouseUp(const MouseEvent&) override;

	// MidiPitchSlider
	void setValue(float v);
    std::function<void(float)> onValueChange;

private:
    float value;
    float clickValue;
};

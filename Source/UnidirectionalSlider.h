#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class UnidirectionalSlider	: public Component
{
public:
    UnidirectionalSlider(Colour c, int& ccn) : colour(c), ccNumber(ccn), value(0.0f) {}
    virtual ~UnidirectionalSlider() = default;

	// Component
	void paint(Graphics&) override;
    void mouseDown(const MouseEvent&) override;

	// UnidirectionalSlider
	void setValue(float v);

private:
    Colour colour;
    int& ccNumber;
    float value;
};

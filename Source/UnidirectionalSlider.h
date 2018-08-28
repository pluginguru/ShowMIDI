#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class UnidirectionalSlider	: public Component
{
public:
    UnidirectionalSlider() : value(0.0f) {}
    virtual ~UnidirectionalSlider() = default;

	// Component
	void paint(Graphics&) override;

	// UnidirectionalSlider
	void setValue(float v);

private:
	float value;
};

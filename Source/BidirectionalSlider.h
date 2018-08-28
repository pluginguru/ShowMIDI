#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class BidirectionalSlider	: public Component
{
public:
    BidirectionalSlider() : value(0.0f) {}
    virtual ~BidirectionalSlider() = default;
    
	// Component
	void paint(Graphics&) override;

	// UnidirectionalSlider
	void setValue(float v);

private:
	float value;
};

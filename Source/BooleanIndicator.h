#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class BooleanIndicator	: public Component
{
public:
    BooleanIndicator() : value(false) {}
    virtual ~BooleanIndicator() = default;

	// Component
	void paint(Graphics&) override;

	// BooleanIndicator
	void setValue(bool v);

private:
	bool value;
};

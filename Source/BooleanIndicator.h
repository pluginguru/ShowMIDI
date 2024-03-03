#pragma once
#include <JuceHeader.h>

class BooleanIndicator	: public Component
{
public:
    BooleanIndicator() : value(false) {}
    virtual ~BooleanIndicator() = default;

	// Component
	void paint(Graphics&) override;

	// BooleanIndicator
	void setValue(bool v);

protected:
	bool value;
};

class TogglingBooleanIndicator : public BooleanIndicator
{
public:
	TogglingBooleanIndicator() : BooleanIndicator() {}

	// Component
	void mouseUp(const MouseEvent&) override;

	// BooleanIndicator
	std::function<void(bool)> onValueChange;
};

#include "BooleanIndicator.h"

void BooleanIndicator::paint (Graphics& g)
{
    auto area = getLocalBounds();

    g.setColour(value ? Colours::violet : Colours::black);
    g.fillRect(area);
}

void BooleanIndicator::setValue(bool v)
{
    value = v;
	repaint();
}

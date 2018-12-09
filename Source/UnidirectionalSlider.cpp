#include "UnidirectionalSlider.h"

void UnidirectionalSlider::paint (Graphics& g)
{
    auto area = getLocalBounds();

    g.setColour(Colours::black);
    g.fillRect(area);
    area.reduce(5, 0);

    if (value > 0.0f)
    {
        g.setColour(colour);
        Rectangle<int> bar(area.getX(),
                           area.getY() + int((1.0f - value) * area.getHeight()),
                           area.getWidth(),
                           int(value * area.getHeight()));
        g.fillRect(bar);
    }

    g.setColour(Colours::white);
    g.drawRect(area);
}

void UnidirectionalSlider::setValue(float v)
{
    value = v;
    if (value < 0.0f) value = 0.0f;
    if (value > 1.0f) value = 1.0f;
	repaint();
}

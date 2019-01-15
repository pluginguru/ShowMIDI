#include "BidirectionalSlider.h"

void BidirectionalSlider::paint (Graphics& g)
{
    auto area = getLocalBounds();

    //g.setColour(Colours::black);
    //g.fillRect(area);

    area.reduce(1, 1);

    if (value > 0.0f)
    {
        g.setColour(Colours::lime);
        Rectangle<int> bar(area.getX(),
                           area.getY() + int((1.0f - value) * 0.5f * area.getHeight()),
                           area.getWidth(),
                           int(value * 0.5f * area.getHeight()));
        g.fillRect(bar);
    }
    else if (value < 0.0f)
    {
        g.setColour(Colours::red);
        Rectangle<int> bar(area.getX(),
                           area.getY() + int(0.5f * area.getHeight()),
                           area.getWidth(),
                           int((-value) * 0.5f * area.getHeight()));
        g.fillRect(bar);
    }

    g.setColour(Colours::white);
    g.drawRect(area);
}

void BidirectionalSlider::setValue(float v)
{
	value = v;
    if (value < -1.0f) value = -1.0f;
    if (value > 1.0f) value = 1.0f;
	repaint();
}

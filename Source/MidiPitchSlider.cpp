#include "MidiPitchSlider.h"

void MidiPitchSlider::paint (Graphics& g)
{
    auto area = getLocalBounds();
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

void MidiPitchSlider::setValue(float v)
{
	value = v;
    if (value < -1.0f) value = -1.0f;
    if (value > 1.0f) value = 1.0f;
	repaint();
}

void MidiPitchSlider::mouseDown(const MouseEvent&)
{
    clickValue = value;
}

void MidiPitchSlider::mouseDrag(const MouseEvent& evt)
{
    float halfHeight = 0.5f * getHeight();
    value = clickValue - evt.getDistanceFromDragStartY() / halfHeight;
    value = jlimit(-1.0f, 1.0f, value);
    if (onValueChange) onValueChange(value);
    repaint();
}

void MidiPitchSlider::mouseUp(const MouseEvent&)
{
    value = clickValue;
    if (onValueChange) onValueChange(value);
    repaint();
}

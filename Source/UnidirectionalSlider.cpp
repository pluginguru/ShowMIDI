#include "UnidirectionalSlider.h"

void UnidirectionalSlider::paint (Graphics& g)
{
    auto area = getLocalBounds();

    //g.setColour(Colours::black);
    //g.fillRect(area);

    area.reduce(1, 1);

    if (value > 0.0f)
    {
        g.setColour(colour);
        int barHeight = int(value * area.getHeight());
        Rectangle<int> bar(area.getX(),
                           area.getY() + area.getHeight() - barHeight,
                           area.getWidth(),
                           barHeight);
        g.fillRect(bar);
    }

    g.setColour(colour.darker());
    g.drawText(String(ccNumber), area.toFloat(), Justification::centredTop);

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

void UnidirectionalSlider::mouseDown(const MouseEvent& evt)
{
    if (evt.mods.isRightButtonDown() || evt.mods.isCtrlDown())
    {
        PopupMenu menu;
        for (int cn = 0; cn < 128; cn++)
        {
            String controllerNameString = "CC" + String(cn);
            const char* controllerName = MidiMessage::getControllerName(cn);
            if (controllerName)
                controllerNameString += " " + String(controllerName);
            menu.addItem(cn + 1, controllerNameString);
        }
        menu.showMenuAsync(PopupMenu::Options(), [this](int id) {
            if (id)
            {
                ccNumber = id - 1;
                repaint();
            }
            });
    }
}

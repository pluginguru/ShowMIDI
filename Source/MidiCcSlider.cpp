#include "MidiCcSlider.h"

void MidiCcSlider::paint (Graphics& g)
{
    auto area = getLocalBounds();

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
    g.drawText(String(inputCc), area.toFloat(), Justification::centredTop);

    g.setColour(Colours::white);
    g.drawRect(area);
}

void MidiCcSlider::setValue(float v)
{
    value = v;
    if (value < 0.0f) value = 0.0f;
    if (value > 1.0f) value = 1.0f;
	repaint();
}

void MidiCcSlider::mouseDown(const MouseEvent& evt)
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
                outputCc = inputCc = id - 1;
                repaint();
            }
            });
        return;
    }

    // Initial click on a slider also sets value
    mouseDrag(evt);
}

void MidiCcSlider::mouseDrag(const MouseEvent& evt)
{
    float height = float(getHeight());
    int mouseY = evt.getPosition().getY();
    value = (height - mouseY) / height;
    value = jlimit(0.0f, 1.0f, value);
    if (onValueChange) onValueChange(value);
    repaint();
}

void MidiCcSlider::mouseWheelMove(const MouseEvent&, const MouseWheelDetails& whd)
{
    float ninc = 0.0f;
    if (whd.deltaY > 0.0f) ninc = 1.0f / 128;
    else if (whd.deltaY < 0.0f) ninc = -1.0f / 128;
    else return;

    value = jlimit(0.0f, 1.0f, value + ninc);
    if (onValueChange) onValueChange(value);
    repaint();
}

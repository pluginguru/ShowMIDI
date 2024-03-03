#pragma once
#include <JuceHeader.h>

class MidiCcSlider	: public Component
{
public:
    MidiCcSlider(Colour c, int& cc)
        : colour(c), inputCc(cc), outputCc(cc), value(0.0f) {}
    MidiCcSlider(Colour c, int& ccin, int& ccout)
        : colour(c), inputCc(ccin), outputCc(ccout), value(0.0f) {}
    virtual ~MidiCcSlider() = default;

	// Component
	void paint(Graphics&) override;
    void mouseDown(const MouseEvent&) override;
    void mouseDrag(const MouseEvent&) override;
    void mouseWheelMove(const MouseEvent&, const MouseWheelDetails&) override;

	// MidiCcSlider
	void setValue(float v);
    std::function<void(float)> onValueChange;

private:
    Colour colour;
    int& inputCc;
    int& outputCc;
    float value;
};
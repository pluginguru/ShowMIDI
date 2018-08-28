#include "PluginProcessor.h"
#include "PluginEditor.h"

KeyboardPluginAudioProcessorEditor::KeyboardPluginAudioProcessorEditor (KeyboardPluginAudioProcessor& p,
                                                                        MidiKeyboardState& keyboardState)
    : AudioProcessorEditor (&p), processor (p)
    , keyboardComponent (keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{
    addAndMakeVisible(pitchWheel);
    pitchWheel.setValue(processor.pitchBend);

    addAndMakeVisible(modWheel);
    modWheel.setValue(processor.modWheel);

    addAndMakeVisible (keyboardComponent);
    keyboardComponent.setKeyWidth(30);

    resizeLimits.setSizeLimits (200, 100, 2000, 200);
    resizer.reset(new ResizableCornerComponent(this, &resizeLimits));
    addAndMakeVisible(*resizer);

    processor.addChangeListener(this);

    setSize (processor.lastUIWidth, processor.lastUIHeight);
}

KeyboardPluginAudioProcessorEditor::~KeyboardPluginAudioProcessorEditor()
{
    processor.removeChangeListener(this);
}

void KeyboardPluginAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}

void KeyboardPluginAudioProcessorEditor::resized()
{
    resizer->setBounds (getWidth() - 16, getHeight() - 16, 16, 16);
    processor.lastUIWidth = getWidth();
    processor.lastUIHeight = getHeight();

    auto area = getLocalBounds();
    pitchWheel.setBounds(area.removeFromLeft(40));
    modWheel.setBounds(area.removeFromLeft(40));

    keyboardComponent.setBounds(area);
    keyboardComponent.setKeyWidth(area.getWidth() / 52.0f);
    keyboardComponent.setLowestVisibleKey(21);
}

void KeyboardPluginAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster*)
{
    pitchWheel.setValue(processor.pitchBend);
    modWheel.setValue(processor.modWheel);
}

#include "PluginProcessor.h"
#include "PluginEditor.h"

KeyboardPluginAudioProcessorEditor::KeyboardPluginAudioProcessorEditor (KeyboardPluginAudioProcessor& p,
                                                                        MidiKeyboardState& keyboardState)
    : AudioProcessorEditor (&p), processor (p)
    , keyboardComponent (keyboardState, MidiKeyboardComponent::horizontalKeyboard)
    , pedalDown(false)
{
    addAndMakeVisible(pitchWheel);
    pitchWheel.setValue(processor.pitchBend);

    addAndMakeVisible(modWheel);
    modWheel.setValue(processor.modWheel);

    addAndMakeVisible(keyboardComponent);
    //keyboardComponent.setKeyWidth(30);

    addAndMakeVisible(sustainPedal);
    sustainPedal.setValue(0);

    size37Button.setButtonText("37 keys");
    size37Button.onClick = [this] { processor.keyCount = 37; resized(); };
    addAndMakeVisible(size37Button);
    size49Button.setButtonText("49 keys");
    size49Button.onClick = [this] { processor.keyCount = 49; resized(); };
    addAndMakeVisible(size49Button);
    size61Button.setButtonText("61 keys");
    size61Button.onClick = [this] { processor.keyCount = 61; resized(); };
    addAndMakeVisible(size61Button);
    size88Button.setButtonText("88 keys");
    size88Button.onClick = [this] { processor.keyCount = 88; resized(); };
    addAndMakeVisible(size88Button);

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
    //g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
    g.fillAll(Colours::black);
}

void KeyboardPluginAudioProcessorEditor::resized()
{
    resizer->setBounds (getWidth() - 16, getHeight() - 16, 16, 16);
    processor.lastUIWidth = getWidth();
    processor.lastUIHeight = getHeight();

    auto area = getLocalBounds();
    pitchWheel.setBounds(area.removeFromLeft(40));
    modWheel.setBounds(area.removeFromLeft(40));

    auto buttonsArea = area.removeFromRight(80);
    buttonsArea.removeFromLeft(5);
    int buttonHeight = (buttonsArea.getHeight() - 6) / 4;
    if (buttonHeight > 24) buttonHeight = 24;
    size37Button.setBounds(buttonsArea.removeFromTop(buttonHeight));
    buttonsArea.removeFromTop(2);
    size49Button.setBounds(buttonsArea.removeFromTop(buttonHeight));
    buttonsArea.removeFromTop(2);
    size61Button.setBounds(buttonsArea.removeFromTop(buttonHeight));
    buttonsArea.removeFromTop(2);
    size88Button.setBounds(buttonsArea.removeFromTop(buttonHeight));

    sustainPedal.setBounds(area.removeFromBottom(6));
    area.removeFromBottom(2);

    keyboardComponent.setBounds(area);
    switch (processor.keyCount)
    {
        case 37:
            keyboardComponent.setKeyWidth(area.getWidth() / 22.0f);
            keyboardComponent.setLowestVisibleKey(48);
            break;
        case 49:
            keyboardComponent.setKeyWidth(area.getWidth() / 29.0f);
            keyboardComponent.setLowestVisibleKey(36);
            break;
        case 61:
            keyboardComponent.setKeyWidth(area.getWidth() / 36.0f);
            keyboardComponent.setLowestVisibleKey(36);
            break;
        default:
            keyboardComponent.setKeyWidth(area.getWidth() / 52.0f);
            keyboardComponent.setLowestVisibleKey(21);
           break;
    }
}

void KeyboardPluginAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster*)
{
    sustainPedal.setValue(processor.sustainPedalDown);
    if (processor.sustainPedalDown && !pedalDown) keyboardComponent.pedalDown();
    else if (!processor.sustainPedalDown && pedalDown) keyboardComponent.pedalUp();
    pedalDown = processor.sustainPedalDown;

    pitchWheel.setValue(processor.pitchBend);
    modWheel.setValue(processor.modWheel);
}

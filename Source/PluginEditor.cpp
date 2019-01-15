#include "PluginProcessor.h"
#include "PluginEditor.h"
#ifdef BRANDED_VERSION
#include "AboutBox.h"
#endif

KeyboardPluginAudioProcessorEditor::KeyboardPluginAudioProcessorEditor (KeyboardPluginAudioProcessor& p,
                                                                        MidiKeyboardState& keyboardState)
    : AudioProcessorEditor(&p)
    , processor(p)
    , keyboardComponent (keyboardState, MidiKeyboardComponent::horizontalKeyboard)
    , modWheel(Colours::hotpink, p.cc1)
    , breathController(Colours::greenyellow, p.cc2)
    , footController(Colours::cyan, p.cc3)
    , softPedal(Colours::darkorange, p.cc4)
    , ccCount(-1)
    , pedalDown(false)
{
    addChildComponent(pitchWheel);
    pitchWheel.setValue(processor.pitchBend);

    addChildComponent(modWheel);
    modWheel.setValue(processor.modWheel);

    addChildComponent(breathController);
    breathController.setValue(processor.breathController);

    addChildComponent(footController);
    footController.setValue(processor.footController);

    addChildComponent(softPedal);
    softPedal.setValue(processor.softPedal);

    addAndMakeVisible(keyboardComponent);

    addAndMakeVisible(sustainPedal);
    sustainPedal.setValue(processor.sustainPedalDown);

    addAndMakeVisible(keyboardButton);
    keyboardButton.setButtonText("61");
    keyboardButton.onClick = [this]
    {
        PopupMenu menu;
        menu.addItem(25, "25 keys");
        menu.addItem(37, "37 keys");
        menu.addItem(49, "49 keys");
        menu.addItem(61, "61 keys");
        menu.addItem(88, "88 keys");
        int kc = menu.show();
        if (kc)
        {
            processor.keyCount = kc;
            keyboardButton.setButtonText(String(kc));
            resized();
        }
    };

    addAndMakeVisible(ccButton);
    ccButton.setButtonText(String(processor.ccCount));
    ccButton.onClick = [this]
    {
        PopupMenu menu;
        menu.addItem(1, "1: Pitch Bend");
        menu.addItem(2, "2: PB + Mod Wheel");
        menu.addItem(5, "5: PB + 4 CCs");
        int ccc = menu.show();
        if (ccc)
        {
            processor.ccCount = ccc;
            ccButton.setButtonText(String(ccc));
            resized();
        }
    };

    resizeLimits.setSizeLimits (400, 100, 2000, 200);
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
#ifdef BRANDED_VERSION
    Image background = ImageCache::getFromMemory(BinaryData::logo_png, BinaryData::logo_pngSize);
    g.drawImageAt(background, 0, 0);
#else
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(Colours::black);
#endif
}

void KeyboardPluginAudioProcessorEditor::mouseDown(const MouseEvent& evt)
{
#ifdef BRANDED_VERSION
    if (evt.getPosition().getX() < 70 && evt.getPosition().getY() < 100)
    {
        AboutBox::modal(processor);
    }
#endif
}

void KeyboardPluginAudioProcessorEditor::resized()
{
    const int buttonsWidth = 36;
    const int maxButtonHeight = 24;
    int sliderWidth = 30;

    if (processor.ccCount != ccCount)
    {
        ccCount = processor.ccCount;
        pitchWheel.setVisible(ccCount >= 1);
        modWheel.setVisible(ccCount >= 2);
        breathController.setVisible(ccCount >= 3);
        footController.setVisible(ccCount >= 4);
        softPedal.setVisible(ccCount >= 5);
    }
    if (ccCount >= 3) sliderWidth = 24;

    resizer->setBounds (getWidth() - 16, getHeight() - 16, 16, 16);
    processor.lastUIWidth = getWidth();
    processor.lastUIHeight = getHeight();

    auto area = getLocalBounds();
#ifdef BRANDED_VERSION
    area.removeFromLeft(72);
#endif
    if (ccCount >= 1) pitchWheel.setBounds(area.removeFromLeft(sliderWidth));
    if (ccCount >= 2) modWheel.setBounds(area.removeFromLeft(sliderWidth));
    if (ccCount >= 3) breathController.setBounds(area.removeFromLeft(sliderWidth));
    if (ccCount >= 4) footController.setBounds(area.removeFromLeft(sliderWidth));
    if (ccCount >= 5) softPedal.setBounds(area.removeFromLeft(sliderWidth));
    if (ccCount >= 1) area.removeFromLeft(3);

    auto buttonsArea = area.removeFromRight(buttonsWidth);
    buttonsArea.removeFromLeft(3);
    int buttonHeight = (buttonsArea.getHeight() - 6) / 4;
    if (buttonHeight > maxButtonHeight) buttonHeight = maxButtonHeight;
    keyboardButton.setBounds(buttonsArea.removeFromTop(buttonHeight).reduced(2));
    ccButton.setBounds(buttonsArea.removeFromTop(buttonHeight).reduced(2));

    sustainPedal.setBounds(area.removeFromBottom(6));
    area.removeFromBottom(2);

    keyboardComponent.setBounds(area);
    switch (processor.keyCount)
    {
        case 25:
            keyboardComponent.setKeyWidth(area.getWidth() / 15.0f);
            keyboardComponent.setLowestVisibleKey(48);
            break;
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

    breathController.setValue(processor.breathController);
    footController.setValue(processor.footController);
    softPedal.setValue(processor.softPedal);
}

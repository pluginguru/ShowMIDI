#include "PluginProcessor.h"
#include "PluginEditor.h"

ShowMidiEditor::ShowMidiEditor (ShowMidiProcessor& p, MyMidiKeyboardState& keyboardState, MidiMessageFifo& fifo)
    : AudioProcessorEditor(&p)
    , processor(p)
    , midiFifo(fifo)
    , keyboardComponent (keyboardState, MidiKeyboardComponent::horizontalKeyboard)
    , ccSlider1(Colours::hotpink, p.cc1)
    , ccSlider2(Colours::greenyellow, p.cc2)
    , ccSlider3(Colours::cyan, p.cc3)
    , ccSlider4(Colours::darkorange, p.cc4)
    , ccCount(-1)
    , pedalDown(false)
{
    addChildComponent(pitchSlider);
    pitchSlider.setValue(processor.pitchBend);
    pitchSlider.onValueChange = [this](float v)
        {
            MidiMessage msg = MidiMessage::pitchWheel(1, int(8191.5f * (v + 1.0f)));
            midiFifo.add(msg);
        };

    addChildComponent(ccSlider1);
    ccSlider1.setValue(processor.cc1Value);
    ccSlider1.onValueChange = [this](float v)
        {
            MidiMessage msg = MidiMessage::controllerEvent(1, processor.cc1, int(127 * v + 0.5f));
            midiFifo.add(msg);
        };

    addChildComponent(ccSlider2);
    ccSlider2.setValue(processor.cc2Value);
    ccSlider2.onValueChange = [this](float v)
        {
            MidiMessage msg = MidiMessage::controllerEvent(1, processor.cc2, int(127 * v + 0.5f));
            midiFifo.add(msg);
        };

    addChildComponent(ccSlider3);
    ccSlider3.setValue(processor.cc3Value);
    ccSlider3.onValueChange = [this](float v)
        {
            MidiMessage msg = MidiMessage::controllerEvent(1, processor.cc3, int(127 * v + 0.5f));
            midiFifo.add(msg);
        };

    addChildComponent(ccSlider4);
    ccSlider4.setValue(processor.cc4Value);
    ccSlider4.onValueChange = [this](float v)
        {
            MidiMessage msg = MidiMessage::controllerEvent(1, processor.cc4, int(127 * v + 0.5f));
            midiFifo.add(msg);
        };

    addAndMakeVisible(keyboardComponent);

    addAndMakeVisible(pedalIndicator);
    pedalIndicator.setValue(processor.sustainPedalDown);

    addAndMakeVisible(keyboardButton);
    keyboardButton.setButtonText(String(processor.keyCount));
    keyboardButton.onClick = [this]
    {
        PopupMenu menu;
        menu.addItem(25, "25 keys");
        menu.addItem(32, "32 keys");
        menu.addItem(37, "37 keys");
        menu.addItem(49, "49 keys");
        menu.addItem(61, "61 keys");
        menu.addItem(73, "73 keys");
        menu.addItem(76, "76 keys");
        menu.addItem(88, "88 keys");
        menu.addItem(128, "-128-");

        menu.showMenuAsync(PopupMenu::Options(), [this](int kc) {
            if (kc)
            {
                processor.keyCount = kc;
                keyboardButton.setButtonText(String(kc));
                resized();
            }
            });
    };

    addAndMakeVisible(ccButton);
    ccButton.setButtonText(String(processor.ccCount));
    ccButton.onClick = [this]
    {
        PopupMenu menu;
        menu.addItem(1, "1: Pitch Bend");
        menu.addItem(2, "2: PB + Mod Wheel");
        menu.addItem(5, "5: PB + 4 CCs");

        menu.showMenuAsync(PopupMenu::Options(), [this](int ccc) {
            if (ccc)
            {
                processor.ccCount = ccc;
                ccButton.setButtonText(String(ccc));
                resized();
            }
            });
    };

    resizeLimits.setSizeLimits (400, 80, 2000, 200);
    resizer.reset(new ResizableCornerComponent(this, &resizeLimits));
    addAndMakeVisible(*resizer);

    processor.addChangeListener(this);

    setSize (processor.lastUIWidth, processor.lastUIHeight);
}

ShowMidiEditor::~ShowMidiEditor()
{
    processor.removeChangeListener(this);
}

void ShowMidiEditor::paint (Graphics& g)
{
    g.fillAll(Colours::black);
}

void ShowMidiEditor::resized()
{
    const int buttonsWidth = 32;
    const int maxButtonHeight = 24;
    int sliderWidth = 22;

    if (processor.ccCount != ccCount)
    {
        ccCount = processor.ccCount;
        pitchSlider.setVisible(ccCount >= 1);
        ccSlider1.setVisible(ccCount >= 2);
        ccSlider2.setVisible(ccCount >= 3);
        ccSlider3.setVisible(ccCount >= 4);
        ccSlider4.setVisible(ccCount >= 5);
    }

    resizer->setBounds (getWidth() - 16, getHeight() - 16, 16, 16);
    processor.lastUIWidth = getWidth();
    processor.lastUIHeight = getHeight();

    auto area = getLocalBounds();
    if (ccCount >= 1) pitchSlider.setBounds(area.removeFromLeft(sliderWidth));
    if (ccCount >= 2) ccSlider1.setBounds(area.removeFromLeft(sliderWidth));
    if (ccCount >= 3) ccSlider2.setBounds(area.removeFromLeft(sliderWidth));
    if (ccCount >= 4) ccSlider3.setBounds(area.removeFromLeft(sliderWidth));
    if (ccCount >= 5) ccSlider4.setBounds(area.removeFromLeft(sliderWidth));
    if (ccCount >= 1) area.removeFromLeft(3);

    auto buttonsArea = area.removeFromRight(buttonsWidth);
    buttonsArea.removeFromLeft(3);
    int buttonHeight = 20;
    if (buttonHeight > maxButtonHeight) buttonHeight = maxButtonHeight;
    keyboardButton.setBounds(buttonsArea.removeFromTop(buttonHeight).reduced(2));
    ccButton.setBounds(buttonsArea.removeFromTop(buttonHeight).reduced(2));

    pedalIndicator.setBounds(area.removeFromBottom(6));
    area.removeFromBottom(2);

    keyboardComponent.setBounds(area);
    switch (processor.keyCount)
    {
    case 25:
        keyboardComponent.setKeyWidth(area.getWidth() / 15.0f);
        keyboardComponent.setAvailableRange(48, 72);
        keyboardComponent.setLowestVisibleKey(48);
        break;
    case 32:
        // Komplete Kontrol m32
        keyboardComponent.setKeyWidth(area.getWidth() / 19.0f);
        keyboardComponent.setAvailableRange(41, 72);
        keyboardComponent.setLowestVisibleKey(41);
        break;
    case 37:
        keyboardComponent.setKeyWidth(area.getWidth() / 22.0f);
        keyboardComponent.setAvailableRange(48, 84);
        keyboardComponent.setLowestVisibleKey(48);
        break;
    case 49:
        keyboardComponent.setKeyWidth(area.getWidth() / 29.0f);
        keyboardComponent.setAvailableRange(36, 84);
        keyboardComponent.setLowestVisibleKey(36);
        break;
    case 61:
        keyboardComponent.setKeyWidth(area.getWidth() / 36.0f);
        keyboardComponent.setAvailableRange(36, 96);
        keyboardComponent.setLowestVisibleKey(36);
        break;
    case 73:
        keyboardComponent.setKeyWidth(area.getWidth() / 43.0f);
        keyboardComponent.setAvailableRange(24, 96);
        keyboardComponent.setLowestVisibleKey(24);
        break;
    case 76:
        keyboardComponent.setKeyWidth(area.getWidth() / 45.0f);
        keyboardComponent.setAvailableRange(21, 96);
        keyboardComponent.setLowestVisibleKey(21);
        break;
    case 128:
        keyboardComponent.setKeyWidth(area.getWidth() / 75.0f);
        keyboardComponent.setAvailableRange(0, 127);
        keyboardComponent.setLowestVisibleKey(0);
        break;
    default:
        keyboardComponent.setKeyWidth(area.getWidth() / 52.0f);
        keyboardComponent.setAvailableRange(21, 108);
        keyboardComponent.setLowestVisibleKey(21);
        break;
    }
}

void ShowMidiEditor::changeListenerCallback(ChangeBroadcaster*)
{
    pedalIndicator.setValue(processor.sustainPedalDown);
    if (processor.sustainPedalDown && !pedalDown) keyboardComponent.pedalDown();
    else if (!processor.sustainPedalDown && pedalDown) keyboardComponent.pedalUp();
    pedalDown = processor.sustainPedalDown;

    pitchSlider.setValue(processor.pitchBend);
    ccSlider1.setValue(processor.cc1Value);

    ccSlider2.setValue(processor.cc2Value);
    ccSlider3.setValue(processor.cc3Value);
    ccSlider4.setValue(processor.cc4Value);
}

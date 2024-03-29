#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ShowMidiProcessor();
}

AudioProcessorEditor* ShowMidiProcessor::createEditor()
{
    return new ShowMidiEditor(*this, keyboardState, midiFifo);
}

ShowMidiProcessor::ShowMidiProcessor()
    : AudioProcessor(BusesProperties().withOutput("Dummy Output", AudioChannelSet::stereo()))
{
    keyCount = 61;
    ccCount = 2;
    cc1 = 1;
    cc2 = 2;
    cc3 = 4;
    cc4 = 67;
    
    lastUIWidth = 600;
    lastUIHeight = 80;

    pitchBend = 0.0f;
    cc1Value = 0.0f;

    cc2Value = 0.0f;
    cc3Value = 0.0f;
    cc4Value = 0.0f;

    sustainPedalDown = false;
}

bool ShowMidiProcessor::isVST() const
{
    return wrapperType == WrapperType::wrapperType_VST ||
           wrapperType == WrapperType::wrapperType_VST3;
}

void ShowMidiProcessor::processBlock (AudioBuffer<float>& audio, MidiBuffer& midi)
{
    MidiMessage msg;
    while (midiFifo.getNumReady())
    {
        midiFifo.read(msg);
        midi.addEvent(msg, 0);
    }

    keyboardState.processNextMidiBuffer(midi, 0, audio.getNumSamples(), true);

    for (auto md : midi)
    {
        msg = md.getMessage();
        if (msg.isPitchWheel())
        {
            pitchBend = float(msg.getPitchWheelValue() - 8192) / 8192.0f;
            sendChangeMessage();
        }
        else if (msg.isControllerOfType(cc1))
        {
            cc1Value = msg.getControllerValue() / 127.0f;
            sendChangeMessage();
        }
        else if (msg.isControllerOfType(cc2))
        {
            cc2Value = msg.getControllerValue() / 127.0f;
            sendChangeMessage();
        }
        else if (msg.isControllerOfType(cc3))
        {
            cc3Value = msg.getControllerValue() / 127.0f;
            sendChangeMessage();
        }
        else if (msg.isControllerOfType(cc4))
        {
            cc4Value = msg.getControllerValue() / 127.0f;
            sendChangeMessage();
        }
        else if (msg.isControllerOfType(64))
        {
            sustainPedalDown = msg.getControllerValue() > 63.0f;
            sendChangeMessage();
        }
    }
}

void ShowMidiProcessor::getStateInformation (MemoryBlock& destData)
{
    XmlElement xml = XmlElement("uiSize");
    xml.setAttribute("keyCount", keyCount);
    xml.setAttribute("ccCount", ccCount);
    xml.setAttribute("cc1", cc1);
    xml.setAttribute("cc2", cc2);
    xml.setAttribute("cc3", cc3);
    xml.setAttribute("cc4", cc4);
    xml.setAttribute("uiWidth", lastUIWidth);
    xml.setAttribute("uiHeight", lastUIHeight);
    copyXmlToBinary(xml, destData);
}

void ShowMidiProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto xml = getXmlFromBinary(data, sizeInBytes);
    keyCount = xml->getIntAttribute("keyCount", keyCount);
    ccCount = xml->getIntAttribute("ccCount", ccCount);
    cc1 = xml->getIntAttribute("cc1", cc1);
    cc2 = xml->getIntAttribute("cc2", cc2);
    cc3 = xml->getIntAttribute("cc3", cc3);
    cc4 = xml->getIntAttribute("cc4", cc4);
    lastUIWidth  = xml->getIntAttribute("uiWidth", lastUIWidth);
    lastUIHeight = xml->getIntAttribute("uiHeight", lastUIHeight);
}

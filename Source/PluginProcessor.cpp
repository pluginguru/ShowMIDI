/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
KeyboardPluginAudioProcessor::KeyboardPluginAudioProcessor()
    : AudioProcessor (BusesProperties()) // add no audio buses at all
{
    keyCount = 61;
    ccCount = 2;
    cc1 = 1;
    cc2 = 2;
    cc3 = 4;
    cc4 = 67;
    
    lastUIWidth = 600;
    lastUIHeight = 100;

    pitchBend = 0.0f;
    modWheel = 0.0f;

    breathController = 0.0f;
    footController = 0.0f;
    softPedal = 0.0f;

    sustainPedalDown = false;
}

KeyboardPluginAudioProcessor::~KeyboardPluginAudioProcessor()
{
}

bool KeyboardPluginAudioProcessor::isVST() const
{
    return wrapperType == WrapperType::wrapperType_VST ||
        wrapperType == WrapperType::wrapperType_VST3;
}

//==============================================================================
void KeyboardPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

void KeyboardPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void KeyboardPluginAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midi)
{
    // the audio buffer in a midi effect will have zero channels!
    jassert (buffer.getNumChannels() == 0);

    MidiMessage msg;
    int ignore;
    for (MidiBuffer::Iterator it (midi); it.getNextEvent (msg, ignore);)
    {
        if (msg.isPitchWheel())
        {
            pitchBend = float(msg.getPitchWheelValue() - 8192) / 8192.0f;
            sendChangeMessage();
        }
        else if (msg.isControllerOfType(cc1))
        {
            modWheel = msg.getControllerValue() / 127.0f;
            sendChangeMessage();
        }
        else if (msg.isControllerOfType(cc2))
        {
            breathController = msg.getControllerValue() / 127.0f;
            sendChangeMessage();
        }
        else if (msg.isControllerOfType(cc3))
        {
            footController = msg.getControllerValue() / 127.0f;
            sendChangeMessage();
        }
        else if (msg.isControllerOfType(cc4))
        {
            softPedal = msg.getControllerValue() / 127.0f;
            sendChangeMessage();
        }
        else if (msg.isControllerOfType(64))
        {
            sustainPedalDown = msg.getControllerValue() > 63.0f;
            sendChangeMessage();
            if (sustainPedalDown) pedalLogic.pedalDown();
            else
            {
                pedalLogic.pedalUp();
                for (int nn = 0; nn < 128; nn++)
                {
                    if (pedalLogic.isNoteSustaining(nn))
                        keyboardState.noteOff(msg.getChannel(), nn, 0);
                }
            }
        }
        else if (msg.isNoteOn())
        {
            pedalLogic.keyDownAction(msg.getNoteNumber());
            keyboardState.noteOn(msg.getChannel(), msg.getNoteNumber(), msg.getVelocity() / 127.0f);
        }
        else if (msg.isNoteOff())
        {
            if (pedalLogic.keyUpAction(msg.getNoteNumber()))
                keyboardState.noteOff(msg.getChannel(), msg.getNoteNumber(), msg.getVelocity() / 127.0f);
        }
    }
}

//==============================================================================
bool KeyboardPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* KeyboardPluginAudioProcessor::createEditor()
{
    return new KeyboardPluginAudioProcessorEditor (*this, keyboardState);
}

//==============================================================================
void KeyboardPluginAudioProcessor::getStateInformation (MemoryBlock& destData)
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

void KeyboardPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    ScopedPointer<XmlElement> xml = getXmlFromBinary(data, sizeInBytes);
    keyCount = xml->getIntAttribute("keyCount", keyCount);
    ccCount = xml->getIntAttribute("ccCount", ccCount);
    cc1 = xml->getIntAttribute("cc1", cc1);
    cc2 = xml->getIntAttribute("cc2", cc2);
    cc3 = xml->getIntAttribute("cc3", cc3);
    cc4 = xml->getIntAttribute("cc4", cc4);
    lastUIWidth  = xml->getIntAttribute("uiWidth", lastUIWidth);
    lastUIHeight = xml->getIntAttribute("uiHeight", lastUIHeight);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KeyboardPluginAudioProcessor();
}

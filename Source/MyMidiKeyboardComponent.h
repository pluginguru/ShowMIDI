#pragma once
#include <JuceHeader.h>
#include "MyMidiKeyboardState.h"

class JUCE_API  MyMidiKeyboardComponent : public KeyboardComponentBase
                                        , private MyMidiKeyboardState::Listener
                                        , private Timer
{
public:
    //==============================================================================
    /** Creates a MyMidiKeyboardComponent.

        @param state        the midi keyboard model that this component will represent
        @param orientation  whether the keyboard is horizontal or vertical
    */
    MyMidiKeyboardComponent (MyMidiKeyboardState& state, Orientation orientation);

    /** Destructor. */
    ~MyMidiKeyboardComponent() override;

    //==============================================================================
    /** Changes the velocity used in midi note-on messages that are triggered by clicking
        on the component.

        Values are 0 to 1.0, where 1.0 is the heaviest.

        @see setMidiChannel
    */
    void setVelocity (float velocity, bool useMousePositionForVelocity);

    //==============================================================================
    /** Changes the midi channel number that will be used for events triggered by clicking
        on the component.

        The channel must be between 1 and 16 (inclusive). This is the channel that will be
        passed on to the MyMidiKeyboardState::noteOn() method when the user clicks the component.

        Although this is the channel used for outgoing events, the component can display
        incoming events from more than one channel - see setMidiChannelsToDisplay()

        @see setVelocity
    */
    void setMidiChannel (int midiChannelNumber);

    /** Returns the midi channel that the keyboard is using for midi messages.
        @see setMidiChannel
    */
    int getMidiChannel() const noexcept            { return midiChannel; }

    /** Sets a mask to indicate which incoming midi channels should be represented by
        key movements.

        The mask is a set of bits, where bit 0 = midi channel 1, bit 1 = midi channel 2, etc.

        If the MyMidiKeyboardState has a key down for any of the channels whose bits are set
        in this mask, the on-screen keys will also go down.

        By default, this mask is set to 0xffff (all channels displayed).

        @see setMidiChannel
    */
    void setMidiChannelsToDisplay (int midiChannelMask);

    /** Returns the current set of midi channels represented by the component.
        This is the value that was set with setMidiChannelsToDisplay().
    */
    int getMidiChannelsToDisplay() const noexcept  { return midiInChannelMask; }

    //==============================================================================
    /** Deletes all key-mappings.

        @see setKeyPressForNote
    */
    void clearKeyMappings();

    /** Maps a key-press to a given note.

        @param key                  the key that should trigger the note
        @param midiNoteOffsetFromC  how many semitones above C the triggered note should
                                    be. The actual midi note that gets played will be
                                    this value + (12 * the current base octave). To change
                                    the base octave, see setKeyPressBaseOctave()
    */
    void setKeyPressForNote (const KeyPress& key, int midiNoteOffsetFromC);

    /** Removes any key-mappings for a given note.

        For a description of what the note number means, see setKeyPressForNote().
    */
    void removeKeyPressForNote (int midiNoteOffsetFromC);

    /** Changes the base note above which key-press-triggered notes are played.

        The set of key-mappings that trigger notes can be moved up and down to cover
        the entire scale using this method.

        The value passed in is an octave number between 0 and 10 (inclusive), and
        indicates which C is the base note to which the key-mapped notes are
        relative.
    */
    void setKeyPressBaseOctave (int newOctaveNumber);

    //==============================================================================
    /** A set of colour IDs to use to change the colour of various aspects of the keyboard.

        These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
        methods.

        @see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
    */
    enum ColourIds
    {
        whiteNoteColourId               = 0x1005000,
        blackNoteColourId               = 0x1005001,
        keySeparatorLineColourId        = 0x1005002,
        mouseOverKeyOverlayColourId     = 0x1005003,  /**< This colour will be overlaid on the normal note colour. */
        keyDownOverlayColourId          = 0x1005004,  /**< This colour will be overlaid on the normal note colour. */
        textLabelColourId               = 0x1005005,
        shadowColourId                  = 0x1005006
    };

    //==============================================================================
    /** Use this method to draw a white note of the keyboard in a given rectangle.

        isOver indicates whether the mouse is over the key, isDown indicates whether the key is
        currently pressed down.

        When doing this, be sure to note the keyboard's orientation.
    */
    virtual void drawWhiteNote (int midiNoteNumber, Graphics& g, Rectangle<float> area,
                                bool isDown, bool isOver, Colour lineColour, Colour textColour);

    /** Use this method to draw a black note of the keyboard in a given rectangle.

        isOver indicates whether the mouse is over the key, isDown indicates whether the key is
        currently pressed down.

        When doing this, be sure to note the keyboard's orientation.
    */
    virtual void drawBlackNote (int midiNoteNumber, Graphics& g, Rectangle<float> area,
                                bool isDown, bool isOver, Colour noteFillColour);

    /** Callback when the mouse is clicked on a key.

        You could use this to do things like handle right-clicks on keys, etc.

        Return true if you want the click to trigger the note, or false if you
        want to handle it yourself and not have the note played.

        @see mouseDraggedToKey
    */
    virtual bool mouseDownOnKey (int midiNoteNumber, const MouseEvent& e);

    /** Callback when the mouse is dragged from one key onto another.

        Return true if you want the drag to trigger the new note, or false if you
        want to handle it yourself and not have the note played.

        @see mouseDownOnKey
    */
    virtual bool mouseDraggedToKey (int midiNoteNumber, const MouseEvent& e);

    /** Callback when the mouse is released from a key.

        @see mouseDownOnKey
    */
    virtual void mouseUpOnKey (int midiNoteNumber, const MouseEvent& e);

    /** Allows text to be drawn on the white notes.

        By default this is used to label the C in each octave, but could be used for other things.

        @see setOctaveForMiddleC
    */
    virtual String getWhiteNoteText (int midiNoteNumber);


    // SD additions
    void pedalDown() {
        setColour(keyDownOverlayColourId, Colours::violet);
    }
    void pedalUp() {
        setColour(keyDownOverlayColourId, Colours::mediumturquoise);
    }


    //==============================================================================
    /** @internal */
    void mouseMove (const MouseEvent&) override;
    /** @internal */
    void mouseDrag (const MouseEvent&) override;
    /** @internal */
    void mouseDown (const MouseEvent&) override;
    /** @internal */
    void mouseUp (const MouseEvent&) override;
    /** @internal */
    void mouseEnter (const MouseEvent&) override;
    /** @internal */
    void mouseExit (const MouseEvent&) override;
    /** @internal */
    void timerCallback() override;
    /** @internal */
    bool keyStateChanged (bool isKeyDown) override;
    /** @internal */
    bool keyPressed (const KeyPress&) override;
    /** @internal */
    void focusLost (FocusChangeType) override;
    /** @internal */
    void colourChanged() override;

private:
    //==============================================================================
    void drawKeyboardBackground (Graphics& g, Rectangle<float> area) override final;
    void drawWhiteKey (int midiNoteNumber, Graphics& g, Rectangle<float> area) override final;
    void drawBlackKey (int midiNoteNumber, Graphics& g, Rectangle<float> area) override final;

    void handleNoteOn  (MyMidiKeyboardState*, int, int, float) override;
    void handleNoteOff (MyMidiKeyboardState*, int, int, float) override;
    void handlePedalDown(MyMidiKeyboardState*, int) override;
    void handlePedalUp(MyMidiKeyboardState*, int) override;

    //==============================================================================
    void resetAnyKeysInUse();
    void updateNoteUnderMouse (Point<float>, bool isDown, int fingerNum);
    void updateNoteUnderMouse (const MouseEvent&, bool isDown);
    void repaintNote (int midiNoteNumber);

    //==============================================================================
    MyMidiKeyboardState& state;
    int midiChannel = 1, midiInChannelMask = 0xffff;
    int keyMappingOctave = 6;

    float velocity = 1.0f;
    bool useMousePositionForVelocity = true;

    Array<int> mouseOverNotes, mouseDownNotes;
    Array<KeyPress> keyPresses;
    Array<int> keyPressNotes;
    BigInteger keysPressed, keysCurrentlyDrawnDown;

    std::atomic<bool> noPendingUpdates { true };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyMidiKeyboardComponent)
};

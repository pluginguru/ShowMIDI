# ShowMIDI
![](showmidi.png)

This is a very simple MIDI effect (can be inserted in the MIDI effects slot of Apple Logic Pro X) to visualize MIDI input activity on a keyboard, pitch and mod wheels. The "AW" version features three extra indicators for MIDI CC's 2, 4, and 67.

I developed this for [John Lehmkuhl](https://www.pluginguru.com/) to use in his [YouTube videos](https://www.youtube.com/channel/UCuKxBfrjXMDZZfuP_bfSHFA), where it's useful to be able to see the actual keyboard input (often quite simple) that results in a particular sound (often complex due to intricate programming).

 * Use buttons at right to set the keyboard size (3-, 4-, 5-octave, or full 88-key piano) and also the number of CC indicators on the left.
 * Right-click (or ctrl-click) any CC indicator to change which CC number it displays.
 * Drag resize grip at bottom right corner to resize the GUI

See also [ShowMIDI_CC](https://github.com/getdunne/ShowMIDI_CC).

Requires [JUCE 5.x](https://shop.juce.com/get-juce), hence subject to the GNU GPL v3 license. Use the *Projucer* to generate the necessary projects, e.g. VST2 for Windows, Audio Unit for Mac.

If you can't or don't want to build these plug-ins from source, see http://getdunne.net/download/showmidi for pre-built versions (with PlugInGuru branding).

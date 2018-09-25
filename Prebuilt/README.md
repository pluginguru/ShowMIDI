# Prebuilt
Here you'll find pre-built plug-ins (click links to download):
 * [ShowMIDI_MacAU_64.zip](https://github.com/getdunne/ShowMIDI/raw/master/Prebuilt/ShowMIDI_MacAU_64.zip) Audio Unit (Mac *.component* file), 64-bit
 * [ShowMIDI_MacVST_64.zip](https://github.com/getdunne/ShowMIDI/raw/master/Prebuilt/ShowMIDI_MacVST_64.zip) VST (v2, Mac *.vst* file), 64-bit
 * [ShowMIDI_WinVST.zip](https://github.com/getdunne/ShowMIDI/raw/master/Prebuilt/ShowMIDI_WinVST.zip) VST(v2, Windows *.dll* files), 32- and 64-bit

On the Mac, put the *.component* file in *~/Library/Audio/Plug-Ins/Components*, and note with recent versions of macOS *you must log out and back in again* before the plug-in will be visible to Logic Pro X and other Audio-Unit hosts.

On the Mac, put the *.vst* file in *~/Library/Audio/Plug-Ins/VST*.

On Windows, put the *.dll* file wherever your VST host program looks for plug-ins.

## Cubase
ShowMIDI is a "MIDI insert effect". It doesn't produce any sound; it simply receives incoming MIDI data, displays the result, and passes it on. Most DAWs allow you to use MIDI insert effect plug-ins, but **Cubase is a notable exception**. Cubase provides a handful of its own MIDI insert effects, and these are the only ones you're allowed to use. [People have been begging Steinberg to support "third-party" MIDI insert effects](https://www.steinberg.net/forums/viewtopic.php?t=111740) for years, to no effect.

For this reason, I created a "VSTi" version of *ShowMIDI*, which pretends to be an ordinary synthesizer plug-in, but doesn't create any sound. In Cubase, you can load this version of the plug-in onto an Instrument track, then just make sure that track's *Record Enable* is set at the same time as whatever other track(s) you're actually recording on, and all will be well.

  * [Click here to download link ShowMIDI_S](https://github.com/getdunne/ShowMIDI/raw/master/Prebuilt/ShowMIDI_S.zip) (64-bit Windows VST *.dll* only)


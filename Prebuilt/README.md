# Prebuilt
Here you'll find pre-built plug-ins (click links to download):
 * [ShowMIDI_MacAU_64.zip](https://github.com/getdunne/ShowMIDI/raw/master/Prebuilt/ShowMIDI_Mac.zip) (AU, VST, VST3) for Macintosh
 * [ShowMIDI_MacVST_64.zip](https://github.com/getdunne/ShowMIDI/raw/master/Prebuilt/ShowMIDI_MacVST_64.zip) (VST, VST3) for Windows, 64-bit
 * [ShowMIDI_WinVST.zip](https://github.com/getdunne/ShowMIDI/raw/master/Prebuilt/ShowMIDI_WinVST.zip) (VST, VST3) for Windows, 32-bit

All three files contain both the basic *ShowMIDI* plug-in, plus the "AW" (Airwave) versions which feature addtional indicators for CC2, CC4, and CC67 as used in the [PlugInGuru "Airwave V3" libraries for Omnisphere 2](https://www.pluginguru.com/products/airwave-v3/).

On the Mac, put the plug-in files into the appropriate sub-folders inside *~/Library/Audio/Plug-Ins*. Note with recent versions of macOS, *you must log out and back in again* before the plug-ins will be visible to Logic Pro X and other hosts.

On Windows, put the *.dll* and/or *.vst3* files wherever your DAW looks for plug-ins. Refer to your particular DAW's documentation for details.

## Cubase
ShowMIDI is a "MIDI insert effect". It doesn't produce any sound; it simply receives incoming MIDI data, displays the result, and passes it on. Most DAWs allow you to use MIDI insert effect plug-ins, but **Cubase is a notable exception**. Cubase provides a handful of its own MIDI insert effects, and these are the only ones you're allowed to use. [People have been begging Steinberg to support "third-party" MIDI insert effects](https://www.steinberg.net/forums/viewtopic.php?t=111740) for years, to no effect.

For this reason, I created a "VSTi" version of *ShowMIDI*, which pretends to be an ordinary synthesizer plug-in, but doesn't create any sound. In Cubase, you can load this version of the plug-in onto an Instrument track, then just make sure that track's *Record Enable* is set at the same time as whatever other track(s) you're actually recording on, and all will be well.

  * [Click here to download link ShowMIDI_S](https://github.com/getdunne/ShowMIDI/raw/master/Prebuilt/ShowMIDI_S.zip) (64-bit Windows VST *.dll* only)

There is no "S" version of the *ShowMIDI_AW*. If you need a tool to ensure that you are receiving the correct MIDI CC messages, use [Cubase's built-in MIDI Monitor plug-in](https://steinberg.help/cubase_plugin_reference/v9/en/_shared/topics/plug_ref/midi_monitor_r.html).

## Acknowledgements

![](VST2LogoWhite.jpg "VST2 logo") ![](VST3LogoBlack.jpg "VST3 logo")

*VST* and its expansion *Virtual Studio Technologies* are registered trade marks of [Steinberg Media Technologies GmbH](https://www.steinberg.net/) and are used under license.

---

![](JUCE-logo.png) ![](gplv3-with-text-136x68.png)

This program was developed with [JUCE 5.x](https://shop.juce.com/get-juce), hence subject to the [GNU GPL v3 license](https://www.gnu.org/licenses/gpl-3.0.en.html). Use the *Projucer* to generate the necessary projects, e.g. VST2 for Windows, Audio Unit for Mac. To build VST/VST3 you'll need the [VST3 SDK](https://github.com/steinbergmedia/vst3sdk) from Steinberg. To distribute finished products, you may need a [license](https://www.steinberg.net/en/company/developers.html).

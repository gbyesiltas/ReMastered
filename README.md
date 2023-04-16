# ReMastered

MIDI controlled harmonizer (prismizer) plugin made using the JUCE framework.
This plugin uses the aubio library for pitch detection and SoundTouch library for pitch shifting. The main processing block is in the `~/Source/PluginProcessor.cpp` file.

If you want to build this project using XCode, you need to import the `aubio.framework` from `./Library/aubio.framework`, and add the correct header path.

If you are using Apple Silicon, unfortunately, you can only build it with Rosetta for the time being. For this, you need to exclude the `arm64` architecture from your builds. The reason for this is because the `aubio.framework` only has a x86 build.

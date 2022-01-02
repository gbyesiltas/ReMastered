# ReMastered
MIDI controlled harmonizer (prismizer) plugin made using the JUCE framework.
This plugin uses the aubio library for pitch detection and SoundTouch library for pitch shifting.

If you want to build this project using XCode, you need to import the `aubio.framework` from ./Library/aubio.framework
If you are using Apple Silicon, unfortunately, you can only build it with Rosetta for the time being.

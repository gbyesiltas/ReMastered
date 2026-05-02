# ReMastered

Realtime MIDI controlled vocal harmonizer (prismizer) plugin made using the JUCE framework.
This plugin uses the aubio library for pitch detection and SoundTouch library for pitch shifting. The main processing block is in the `Source/PluginProcessor.cpp` file.

---

## Setup & Build Guide (macOS)

This guide takes you from a fresh Mac to a working AU/VST3 plugin in Ableton (or any AU/VST3 host), and a Standalone app for quick testing without a DAW.

### 1. Install Xcode Command Line Tools

This gives you `clang`, `git`, and the macOS SDK — everything needed to compile.

```bash
xcode-select --install
```

A dialog will appear asking you to install. Click **Install** and wait for it to finish (~5–10 min). Verify with:

```bash
clang --version
```

### 2. Install Homebrew (package manager)

If you don't have it already:

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

Follow the instructions at the end of the installer — it will ask you to run two commands to add Homebrew to your PATH. Do those before continuing.

### 3. Install CMake

```bash
brew install cmake
```

Verify:

```bash
cmake --version   # should print 3.22 or higher
```

### 4. Clone the repository

```bash
git clone https://github.com/gbyesiltas/ReMastered.git
cd ReMastered
```

### 5. Build the plugin

The build script configures CMake, automatically downloads JUCE (no manual clone needed), compiles everything, and installs the AU and VST3 into your system plugin folders.

```bash
chmod +x build-macos-plugin.sh
./build-macos-plugin.sh
```

The first run will take a few minutes while JUCE downloads and compiles. Subsequent builds are fast.

When it finishes you will see:

```
Build complete. Installed plugin locations:
  VST3: ~/Library/Audio/Plug-Ins/VST3/ReMastered.vst3
  AU:   ~/Library/Audio/Plug-Ins/Components/ReMastered.component
```

### 6. Test without a DAW (Standalone)

A self-contained app is built alongside the plugin. Run it directly:

```bash
open build/ReMastered_artefacts/Release/Standalone/ReMastered.app
```

Or use the helper script:

```bash
chmod +x plugin-safety.sh
./plugin-safety.sh standalone
```

The Standalone app accepts audio from any input device and MIDI from any connected controller. Use it to verify the harmonizer works before loading it into a DAW.

### 7. Load in Ableton Live (or any AU/VST3 host)

**AU (recommended for Ableton on macOS):**

1. Open Ableton Live.
2. Go to **Preferences → Plug-Ins** and make sure **Use Audio Units** is enabled.
3. Click **Rescan** if ReMastered doesn't appear immediately.
4. Find **ReMastered** under Audio Units → GBY in your plugin browser and drag it onto an audio track.

**VST3:**

1. In Ableton **Preferences → Plug-Ins**, add `~/Library/Audio/Plug-Ins/VST3` to your VST3 folder list if it isn't there already.
2. Rescan and find **ReMastered** under VST3 → GBY.

### 8. Using the plugin

- **Sing or route audio** into the track with ReMastered on it.
- **Hold MIDI notes** to set the harmony target manually, or enable **Auto Harmony** and let the engine detect the scale from your pitch.
- In Auto Harmony mode, set the **Key**, **Mode**, and **Style** to define the harmonic context. Adjust **Voices**, **Spread**, **Detune**, and **Glide** to shape the sound.
- Use the **FX panel** (dry mix, reverb, delay, drive, clarity) to blend the harmonized signal with the dry input.

### Troubleshooting

| Problem | Fix |
|---|---|
| Plugin doesn't appear in Ableton | Run `./plugin-safety.sh status` to confirm it's installed, then rescan in Ableton Preferences |
| AU validation error | Run `auval -v aufx RmS1 GbY1` in Terminal to see the full validation output |
| Build fails with "CMake version too old" | Run `brew upgrade cmake` |
| Build fails with Xcode SDK error | Run `sudo xcode-select --reset` then retry |
| Need to quickly disable the plugin | Run `./plugin-safety.sh disable` — moves it out of the active plugin folder without deleting it. Use `enable` to restore. |

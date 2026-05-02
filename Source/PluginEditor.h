#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class ReMasteredAudioProcessorEditor : public AudioProcessorEditor
{
public:
    explicit ReMasteredAudioProcessorEditor(ReMasteredAudioProcessor&);
    ~ReMasteredAudioProcessorEditor() override;

    void paint(Graphics&) override;
    void resized() override;

private:
    void configureSlider(juce::Slider& slider,
                         double min,
                         double max,
                         double step,
                         const juce::String& suffix,
                         std::function<void(float)> setter,
                         float initial);

    void configureCombo(juce::ComboBox& combo,
                        const juce::StringArray& items,
                        int selected,
                        std::function<void(int)> setter);

    ReMasteredAudioProcessor& audioProcessor;

    AudioVisualiserComponent audioVisualiser { 1 };
    MidiKeyboardComponent midiKeyboardComponent;

    juce::ToggleButton autoHarmonyToggle { "Auto Harmony" };

    juce::ComboBox keyBox;
    juce::ComboBox modeBox;
    juce::ComboBox styleBox;

    juce::Slider voiceSlider;
    juce::Slider harmonySlider;
    juce::Slider spreadSlider;
    juce::Slider detuneSlider;
    juce::Slider glideSlider;
    juce::Slider motionRateSlider;
    juce::Slider motionDepthSlider;

    juce::Slider drySlider;
    juce::Slider gainSlider;
    juce::Slider driveSlider;
    juce::Slider delayMixSlider;
    juce::Slider delayTimeSlider;
    juce::Slider feedbackSlider;
    juce::Slider reverbSlider;
    juce::Slider claritySlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReMasteredAudioProcessorEditor)
};

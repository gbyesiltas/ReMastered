#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

namespace
{
const juce::StringArray kKeyNames { "C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B" };
const juce::StringArray kModeNames { "Ionian", "Dorian", "Phrygian", "Lydian", "Mixolydian", "Aeolian", "Harmonic Minor", "Phrygian Dom" };
const juce::StringArray kStyleNames { "Gentle", "Bon Layer", "Wide Stack", "Cluster", "Melodic" };
}

ReMasteredAudioProcessorEditor::ReMasteredAudioProcessorEditor(ReMasteredAudioProcessor& p)
    : AudioProcessorEditor(&p),
      audioProcessor(p),
      midiKeyboardComponent(audioProcessor.getKeyboardState(), juce::MidiKeyboardComponent::horizontalKeyboard)
{
    setSize(920, 760);
    setOpaque(true);

    audioVisualiser.setColours(juce::Colours::black, juce::Colour::fromRGB(120, 220, 170));
    audioVisualiser.setRepaintRate(50);
    audioVisualiser.setBufferSize(512);
    audioVisualiser.setSamplesPerBlock(32);
    addAndMakeVisible(audioVisualiser);
    audioProcessor.setVisualiserTarget(&audioVisualiser);

    midiKeyboardComponent.setEnabled(true);
    midiKeyboardComponent.setKeyWidth(20.0f);
    addAndMakeVisible(midiKeyboardComponent);

    autoHarmonyToggle.setToggleState(audioProcessor.getAutoHarmonyEnabled(), juce::dontSendNotification);
    autoHarmonyToggle.onClick = [this]
    {
        audioProcessor.setAutoHarmonyEnabled(autoHarmonyToggle.getToggleState());
    };
    addAndMakeVisible(autoHarmonyToggle);

    configureCombo(keyBox, kKeyNames, audioProcessor.getKeyRoot(),
                   [this](int selected) { audioProcessor.setKeyRoot(selected); });
    configureCombo(modeBox, kModeNames, audioProcessor.getModeIndex(),
                   [this](int selected) { audioProcessor.setModeIndex(selected); });
    configureCombo(styleBox, kStyleNames, audioProcessor.getStyleIndex(),
                   [this](int selected) { audioProcessor.setStyleIndex(selected); });

    configureSlider(voiceSlider, 1.0, 6.0, 1.0, " voices",
                    [this](float value) { audioProcessor.setVoiceCount(static_cast<int>(std::round(value))); },
                    static_cast<float>(audioProcessor.getVoiceCount()));

    configureSlider(harmonySlider, 0.0, 1.0, 0.01, " harmony",
                    [this](float value) { audioProcessor.setHarmonyAmount(value); },
                    audioProcessor.getHarmonyAmount());

    configureSlider(spreadSlider, 0.0, 1.0, 0.01, " spread",
                    [this](float value) { audioProcessor.setSpreadAmount(value); },
                    audioProcessor.getSpreadAmount());

    configureSlider(detuneSlider, 0.0, 40.0, 0.1, " cents",
                    [this](float value) { audioProcessor.setDetuneCents(value); },
                    audioProcessor.getDetuneCents());

    configureSlider(glideSlider, 5.0, 350.0, 1.0, " ms",
                    [this](float value) { audioProcessor.setGlideMs(value); },
                    audioProcessor.getGlideMs());

    configureSlider(motionRateSlider, 0.0, 8.0, 0.01, " Hz",
                    [this](float value) { audioProcessor.setMotionRateHz(value); },
                    audioProcessor.getMotionRateHz());

    configureSlider(motionDepthSlider, 0.0, 1.0, 0.01, " depth",
                    [this](float value) { audioProcessor.setMotionDepth(value); },
                    audioProcessor.getMotionDepth());

    configureSlider(drySlider, 0.0, 1.0, 0.01, " dry",
                    [this](float value) { audioProcessor.setDryMix(value); },
                    audioProcessor.getDryMix());

    configureSlider(gainSlider, 0.0, 2.0, 0.01, "x",
                    [this](float value) { audioProcessor.setOutputGain(value); },
                    audioProcessor.getOutputGain());

    configureSlider(driveSlider, 0.0, 1.0, 0.01, " drive",
                    [this](float value) { audioProcessor.setDriveAmount(value); },
                    audioProcessor.getDriveAmount());

    configureSlider(delayMixSlider, 0.0, 0.95, 0.01, " dly mix",
                    [this](float value) { audioProcessor.setDelayMix(value); },
                    audioProcessor.getDelayMix());

    configureSlider(delayTimeSlider, 40.0, 900.0, 1.0, " ms",
                    [this](float value) { audioProcessor.setDelayTimeMs(value); },
                    audioProcessor.getDelayTimeMs());

    configureSlider(feedbackSlider, 0.0, 0.93, 0.01, " feedback",
                    [this](float value) { audioProcessor.setDelayFeedback(value); },
                    audioProcessor.getDelayFeedback());

    configureSlider(reverbSlider, 0.0, 1.0, 0.01, " reverb",
                    [this](float value) { audioProcessor.setReverbMix(value); },
                    audioProcessor.getReverbMix());

    configureSlider(claritySlider, 0.0, 1.0, 0.01, " clarity",
                    [this](float value) { audioProcessor.setClarity(value); },
                    audioProcessor.getClarity());
}

ReMasteredAudioProcessorEditor::~ReMasteredAudioProcessorEditor()
{
    audioProcessor.setVisualiserTarget(nullptr);
}

void ReMasteredAudioProcessorEditor::paint(Graphics& g)
{
    const auto bg = juce::Colour::fromRGB(19, 22, 26);
    const auto panel = juce::Colour::fromRGB(30, 36, 42);
    const auto panel2 = juce::Colour::fromRGB(21, 27, 33);

    g.fillAll(bg);

    auto area = getLocalBounds().reduced(10);
    auto top = area.removeFromTop(juce::jlimit(200, 380, static_cast<int>(area.getHeight() * 0.50f)));
    auto left = top.removeFromLeft(top.getWidth() / 2).reduced(6);
    auto right = top.reduced(6);

    g.setColour(panel);
    g.fillRoundedRectangle(left.toFloat(), 10.0f);
    g.setColour(panel2);
    g.fillRoundedRectangle(right.toFloat(), 10.0f);

    g.setColour(juce::Colours::white.withAlpha(0.88f));
    g.setFont(juce::FontOptions(18.0f, juce::Font::bold));
    g.drawText("Harmony", left.removeFromTop(28), juce::Justification::centredLeft, false);
    g.drawText("FX", right.removeFromTop(28), juce::Justification::centredLeft, false);
}

void ReMasteredAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(10);

    const int topHeight = juce::jlimit(200, 380, static_cast<int>(area.getHeight() * 0.50f));
    auto top = area.removeFromTop(topHeight);
    auto leftPanel = top.removeFromLeft(top.getWidth() / 2).reduced(10);
    auto rightPanel = top.reduced(10);

    auto leftHeader = leftPanel.removeFromTop(34);
    juce::ignoreUnused(leftHeader);

    auto toggleRow = leftPanel.removeFromTop(28);
    autoHarmonyToggle.setBounds(toggleRow.removeFromLeft(180));

    auto comboRow = leftPanel.removeFromTop(36);
    keyBox.setBounds(comboRow.removeFromLeft(120).reduced(2));
    modeBox.setBounds(comboRow.removeFromLeft(210).reduced(2));
    styleBox.setBounds(comboRow.reduced(2));

    auto harmonyGrid = leftPanel.reduced(0, 4);
    const int hRow = 34;

    voiceSlider.setBounds(harmonyGrid.removeFromTop(hRow));
    harmonySlider.setBounds(harmonyGrid.removeFromTop(hRow));
    spreadSlider.setBounds(harmonyGrid.removeFromTop(hRow));
    detuneSlider.setBounds(harmonyGrid.removeFromTop(hRow));
    glideSlider.setBounds(harmonyGrid.removeFromTop(hRow));
    motionRateSlider.setBounds(harmonyGrid.removeFromTop(hRow));
    motionDepthSlider.setBounds(harmonyGrid.removeFromTop(hRow));

    auto rightHeader = rightPanel.removeFromTop(34);
    juce::ignoreUnused(rightHeader);

    auto fxGrid = rightPanel.reduced(0, 6);
    const int fxRow = 34;
    drySlider.setBounds(fxGrid.removeFromTop(fxRow));
    gainSlider.setBounds(fxGrid.removeFromTop(fxRow));
    driveSlider.setBounds(fxGrid.removeFromTop(fxRow));
    delayMixSlider.setBounds(fxGrid.removeFromTop(fxRow));
    delayTimeSlider.setBounds(fxGrid.removeFromTop(fxRow));
    feedbackSlider.setBounds(fxGrid.removeFromTop(fxRow));
    reverbSlider.setBounds(fxGrid.removeFromTop(fxRow));
    claritySlider.setBounds(fxGrid.removeFromTop(fxRow));

    const int keyboardHeight = juce::jlimit(64, 104, static_cast<int>(area.getHeight() * 0.26f));
    auto visualArea = area.removeFromTop(juce::jmax(120, area.getHeight() - keyboardHeight)).reduced(4);
    audioVisualiser.setBounds(visualArea);
    midiKeyboardComponent.setBounds(area.removeFromTop(keyboardHeight).reduced(4));
}

void ReMasteredAudioProcessorEditor::configureSlider(juce::Slider& slider,
                                                     double min,
                                                     double max,
                                                     double step,
                                                     const juce::String& suffix,
                                                     std::function<void(float)> setter,
                                                     float initial)
{
    slider.setSliderStyle(juce::Slider::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 22);
    slider.setRange(min, max, step);
    slider.setTextValueSuffix(suffix);
    slider.setValue(initial, juce::dontSendNotification);
    slider.onValueChange = [setter, &slider]
    {
        setter(static_cast<float>(slider.getValue()));
    };
    addAndMakeVisible(slider);
}

void ReMasteredAudioProcessorEditor::configureCombo(juce::ComboBox& combo,
                                                    const juce::StringArray& items,
                                                    int selected,
                                                    std::function<void(int)> setter)
{
    combo.clear();
    for (int i = 0; i < items.size(); ++i)
        combo.addItem(items[i], i + 1);

    combo.setSelectedItemIndex(juce::jmax(0, selected), juce::dontSendNotification);
    combo.onChange = [setter, &combo]
    {
        setter(combo.getSelectedItemIndex());
    };

    addAndMakeVisible(combo);
}

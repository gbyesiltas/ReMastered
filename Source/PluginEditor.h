#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//This file holds the information mainly for the visuals of the plugin

class ReMasteredAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    ReMasteredAudioProcessorEditor (ReMasteredAudioProcessor&);
    ~ReMasteredAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    //==============================================================================
    AudioVisualiserComponent* audioVisualiser;
    MidiKeyboardComponent* midiKeyboardComponent;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReMasteredAudioProcessorEditor)
};

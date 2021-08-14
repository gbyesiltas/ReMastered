#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ReMasteredAudioProcessorEditor::ReMasteredAudioProcessorEditor (ReMasteredAudioProcessor& p)
    : AudioProcessorEditor (&p)
{
    setSize(500, 300);
    setOpaque(true);
    this->audioVisualiser = p.getAudioVisualiserComponent();
    this->audioVisualiser->setBounds(0,0,500, 250);
    this->audioVisualiser->setColours(Colours::black,Colours::violet);
    this->audioVisualiser->setRepaintRate(30);
    
    this->midiKeyboardComponent->setBounds(0, 250, 500, 50);
    //TODO: Make this keyboard component actually interactive
    addAndMakeVisible(this->audioVisualiser);
    addAndMakeVisible(this->midiKeyboardComponent);
}

ReMasteredAudioProcessorEditor::~ReMasteredAudioProcessorEditor()
{
    this->midiKeyboardComponent->~MidiKeyboardComponent();
}

//==============================================================================
void ReMasteredAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll(Colours::black);
    g.setOpacity(1.0f);
    g.setColour(Colours::white);
}

void ReMasteredAudioProcessorEditor::resized()
{
}

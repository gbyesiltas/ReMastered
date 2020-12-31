#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ReMasteredAudioProcessorEditor::ReMasteredAudioProcessorEditor (ReMasteredAudioProcessor& p)
    : AudioProcessorEditor (&p)
{
    setSize(500, 300);
    setOpaque(true);
    this->audioVisualizer = p.getAudioVisualiserComponent();
    this->audioVisualizer->setBounds(0,0,500, 300);
    this->audioVisualizer->setColours(Colours::black,Colours::violet);
    this->audioVisualizer->setRepaintRate(30);
    
    addAndMakeVisible(this->audioVisualizer);
}

ReMasteredAudioProcessorEditor::~ReMasteredAudioProcessorEditor()
{
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

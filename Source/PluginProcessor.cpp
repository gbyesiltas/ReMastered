#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <vector>

//==============================================================================
ReMasteredAudioProcessor::ReMasteredAudioProcessor() 
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::mono(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::mono(), true)
                     #endif
                       )
#endif
{
    initSoundTouch();
    audioVisualiser->setBufferSize(128);
    audioVisualiser->setSamplesPerBlock(16);
}

ReMasteredAudioProcessor::~ReMasteredAudioProcessor()
{
    clearStProcessors(true);
    del_aubio_pitch(aubioPitchDetector);
    del_fvec(aubioInput);
    del_fvec(aubioResult);
    audioVisualiser->clear();
    audioVisualiser->~AudioVisualiserComponent();
    for (int i = 0; i < ST_PROCESSOR_NUMBER; i++) {
        st_buf[i].clear();
    }
    
}

//==============================================================================
const juce::String ReMasteredAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ReMasteredAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ReMasteredAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ReMasteredAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ReMasteredAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ReMasteredAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ReMasteredAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ReMasteredAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ReMasteredAudioProcessor::getProgramName (int index)
{
    return {};
}

void ReMasteredAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ReMasteredAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    if(this->aubioPitchDetector != nullptr) del_aubio_pitch(aubioPitchDetector);
    aubioPitchDetector = new_aubio_pitch("default",
                                         this->pitchDetectionWindowSize,
                                         samplesPerBlock,
                                         this->sampleRate);
    clearStProcessors(false);
    st_buf.resize(ST_PROCESSOR_NUMBER);
    for (int i = 0; i < ST_PROCESSOR_NUMBER; i++) {
        st_buf[i].clear();
        st_buf[i].resize(samplesPerBlock);
    }
    if (this->sampleRate != sampleRate) {
        for (int processorIndex = 0; processorIndex < ST_PROCESSOR_NUMBER; processorIndex++) {
            stProcessors[processorIndex]->setSampleRate(sampleRate);
        }
        this->sampleRate = sampleRate;
    }
}

void ReMasteredAudioProcessor::releaseResources()
{
    clearStProcessors(false);
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ReMasteredAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ReMasteredAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    const float* bufferStart = buffer.getReadPointer(0);
    int numSamples = buffer.getNumSamples();
    
    int time;
    MidiMessage m;
    
    for (MidiBuffer::Iterator i(midiMessages); i.getNextEvent(m, time);)
    {
        if (currentDetectedFrequency == 0) break;
        handleMidiMessage(m);
    }

    std::vector<float> read_buf(bufferStart,bufferStart+numSamples);
    for (int i = 0; i < numSamples; ++i) {
        fvec_set_sample(this->aubioInput,buffer.getSample(0,i),(aubioIndex*numSamples)+i);
    }
    aubioIndex = aubioIndex+1;
    if(aubioIndex == (pitchDetectionWindowSize/numSamples)) aubioIndex = 0;

    aubio_pitch_do(aubioPitchDetector, aubioInput, aubioResult);
    float detectedFrequency = fvec_get_sample(aubioResult,0);
    if(currentDetectedFrequency!=detectedFrequency && detectedFrequency != 0){
        if((currentDetectedFrequency==-1) || (currentDetectedFrequency!=-1 && detectedFrequency<=currentDetectedFrequency*3)){
            softwareCounter=0;
            singerOnHold=false;
            currentDetectedFrequency=detectedFrequency;
            for (int i = 0; i < ST_PROCESSOR_NUMBER; i++) {
                if (!stProcessorActive[i]) continue;
                autoTuneFreqST(stProcessorPlaying[i], currentDetectedFrequency, i);
            }
        }
        else if(detectedFrequency == 0) softwareCounter++;
        
        if(softwareCounter>=2 && !singerOnHold){
            singerOnHold=true;
            audioVisualiser->clear();
            for (int processorIndex = 0; processorIndex < ST_PROCESSOR_NUMBER; processorIndex++) {
                if (!stProcessorActive[processorIndex])continue;
                stProcessors[processorIndex]->clear();
                st_buf[processorIndex].clear();
            }
        }
    }

    if(!singerOnHold){
        
        for (int processorIndex = 0; processorIndex < ST_PROCESSOR_NUMBER; processorIndex++) {
            if (!stProcessorActive[processorIndex])continue;
            stProcessors[processorIndex]->putSamples(read_buf.data(), numSamples);  //inputting the data to the processor
            int numReadySamples = stProcessors[processorIndex]->numSamples();
            if (numReadySamples < numSamples) continue;
            stProcessors[processorIndex]->receiveSamples(st_buf[processorIndex].data(), numSamples); //getting the processed data if there is enough
        }
        
        for (int i = 0; i < numSamples; i++) {
            for (int processorIndex = 0; processorIndex < ST_PROCESSOR_NUMBER; processorIndex++) {
                if (!stProcessorActive[processorIndex]) continue;
                buffer.addSample(0, i, st_buf[processorIndex][i] * stProcessorVolume[processorIndex]);
            }
        }
        audioVisualiser->pushBuffer(buffer);
    }
}

//==============================================================================
bool ReMasteredAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ReMasteredAudioProcessor::createEditor()
{
    return new ReMasteredAudioProcessorEditor (*this);
}

//==============================================================================
void ReMasteredAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ReMasteredAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReMasteredAudioProcessor();
}

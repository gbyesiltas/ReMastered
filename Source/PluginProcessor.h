#pragma once

#include <JuceHeader.h>
#include "SoundTouch.h"
#include <vector>
#include <aubio.h>
#include <pitch/pitch.h>

#define ST_PROCESSOR_NUMBER 6
#define CHANNEL_NUMBER 1
#define SCOPE_SIZE 512

#define NO_PROCESSOR_INPUT -1
#define NO_CURRENT_FREQUENCY -2
#define NO_DESIRED_FREQUENCY -3

//==============================================================================
/**
 */
class ReMasteredAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    ReMasteredAudioProcessor();
    ~ReMasteredAudioProcessor();
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    
    //==============================================================================
    
#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif
    
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    
    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    
    //==============================================================================
    const String getName() const override;
    
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    
    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;
    
    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    void initSoundTouch() {
        for (int processorIndex = 0; processorIndex < ST_PROCESSOR_NUMBER; processorIndex++) {
            setStProcessor(processorIndex, new soundtouch::SoundTouch());
            getStProcessor(processorIndex)->setSampleRate(sampleRate);
            getStProcessor(processorIndex)->setChannels(CHANNEL_NUMBER);
            getStProcessor(processorIndex)->setPitch(1);
            getStProcessor(processorIndex)->setSetting(SETTING_USE_QUICKSEEK, 0);  //quick algorithm mode
            setStProcessorActive(processorIndex, false);
        }
    }
    soundtouch::SoundTouch* getStProcessor(int processorIndex) {
        return stProcessors[processorIndex];
    }
    void setStProcessor(int processorIndex, soundtouch::SoundTouch* newStProcessor) {
        if (processorIndex < ST_PROCESSOR_NUMBER) stProcessors[processorIndex] = newStProcessor;
    }
    
    int getFirstActiveSt() {
        for (int i = 0; i < ST_PROCESSOR_NUMBER; i++) {
            if (stProcessorActive[i]) return i;
        }
        return -1;
    }
    int getFirstFreeSt() {
        for (int i = 0; i < ST_PROCESSOR_NUMBER; i++) {
            if (stProcessorActive[i] == false) return i;
        }
        return -1;
    }
    int getNumberOfActiveSt() {
        int num = 0;
        for (int i = 0; i < ST_PROCESSOR_NUMBER; i++) {
            if (stProcessorActive[i]) num++;
        }
        return num;
    }
    int findStProcessorPlaying(float noteInHz) {
        for (int i = 0; i < ST_PROCESSOR_NUMBER; i++) {
            if (stProcessorPlaying[i] == noteInHz && stProcessorActive[i]) return i;
        }
        return -1;
    }
    
    bool isStProcessorActive(int processorIndex) {
        return stProcessorActive[processorIndex];
    }
    void setStProcessorActive(int processorIndex, bool active) {
        stProcessorActive[processorIndex] = active;
    }
    void freeStProcessor(int processorIndex) {
        if (processorIndex == -1) return;
        
        stProcessors[processorIndex]->clear();
        stProcessorActive[processorIndex] = false;
    }
    void clearStProcessors(bool deleteProcessors) {
        for (int processorIndex = 0; processorIndex < ST_PROCESSOR_NUMBER; processorIndex++) {
            stProcessors[processorIndex]->clear();
            if(deleteProcessors) delete stProcessors[processorIndex];
        }
    }
    
    bool canActiveProcessorsOutput(int numSamplesToOutput) {
        bool enoughSamples = true;
        bool atLeastOneActiveProcessor = false;
        for (int processorIndex = 0; processorIndex < ST_PROCESSOR_NUMBER; processorIndex++) {
            if (stProcessorActive[processorIndex]) {
                if (stProcessors[processorIndex]->numSamples() < numSamplesToOutput) enoughSamples = false;
                atLeastOneActiveProcessor = true;
            }
        }
        return enoughSamples && atLeastOneActiveProcessor;
    }
    
    //===============================================================================
    
    int autoTuneFreqST(float desiredFrequency, float currentFrequency, int stProcessorIndex) {
        if (currentFrequency == -1) {
            DBG("No current frequency");
            return NO_CURRENT_FREQUENCY;
        }
        
        if (stProcessorIndex == -1) {
            DBG("No processor input");
            return NO_PROCESSOR_INPUT;
        }
        
        if (desiredFrequency == 0) {
            DBG("No desired frequency");
            return NO_DESIRED_FREQUENCY;
        }
        
        float correctionFactor = desiredFrequency / currentFrequency;
        stProcessors[stProcessorIndex]->setPitch(correctionFactor);
        return 1;
    }
    
    //=================================================================================
    
    AudioVisualiserComponent* getAudioVisualiserComponent(){ return this->audioVisualiser; }
    MidiKeyboardComponent* getMidiKeyboardComponent(){ return this->midiKeyboardComponent; }
    
    //=================================================================================
    
    void handleMidiMessage(MidiMessage m){
        if (m.isNoteOn())
        {
            int firstFreeSt = getFirstFreeSt();
            if (firstFreeSt != -1) {
                autoTuneFreqST(MidiMessage::getMidiNoteInHertz(m.getNoteNumber()), currentDetectedFrequency, firstFreeSt);
                stProcessorActive[firstFreeSt] = true;
                stProcessorVolume[firstFreeSt] = m.getFloatVelocity();
                stProcessorPlaying[firstFreeSt] = MidiMessage::getMidiNoteInHertz(m.getNoteNumber());
            }
        }
        else if (m.isNoteOff())
        {
            int stProcessorIndex = findStProcessorPlaying(MidiMessage::getMidiNoteInHertz(m.getNoteNumber()));
            freeStProcessor(stProcessorIndex);
        }
    }
    
private:
    soundtouch::SoundTouch* stProcessors[ST_PROCESSOR_NUMBER];
    bool stProcessorActive[ST_PROCESSOR_NUMBER];
    float stProcessorPlaying[ST_PROCESSOR_NUMBER];
    float stProcessorVolume[ST_PROCESSOR_NUMBER];
    //==============================================================================
    int sampleRate = 44100; //default value, can be changed
    float currentDetectedFrequency = -1;
    float currentNoteIndex = -1;
    float currentGoalFrequency = 0.0f;
    //==============================================================================
    std::vector<std::vector<float>> st_buf;
    int softwareCounter = 0;
    bool singerOnHold = false;
    
    float noteFrequencies[73] = { 16.35,17.32,18.35,19.45,20.60,21.83,23.12,24.50,
        25.96,27.50,29.14,30.87,32.70,34.65,36.71,38.89,41.20,43.65,46.25,49.00,
        51.91,55.00,58.27,61.74,65.41,69.30,73.42,77.78,82.41,87.31,92.50,98.00,
        103.83,110.00,116.54,123.47,130.81,138.59,146.83,155.56,164.81,174.61,
        185.00,196.00,207.65,220.00,233.08,246.94,261.63,277.18,293.66,311.13,
        329.63,349.23,369.99,392.00,415.30,440.00,466.16,493.88,523.25,554.37,
        587.33,622.25,659.26,698.46,739.99,783.99,830.61,880.00,932.33,987.77,1046.50 };
    //==============================================================================
    int pitchDetectionWindowSize = 4096;
    int aubioIndex = 0; // pitch detection window size can be higher than the numSamples here, so using this to get the correct FFT sample index for pitch detection
    aubio_pitch_t* aubioPitchDetector;
    fvec_t* aubioInput = new_fvec(this->pitchDetectionWindowSize);
    fvec_t* aubioResult = new_fvec(1);
    //===============================================================================
    AudioVisualiserComponent* audioVisualiser = new AudioVisualiserComponent(1);
    juce::MidiKeyboardState keyboardState;
    MidiKeyboardComponent* midiKeyboardComponent = new MidiKeyboardComponent(keyboardState,juce::MidiKeyboardComponent::horizontalKeyboard);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReMasteredAudioProcessor)
};

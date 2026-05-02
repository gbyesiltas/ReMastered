#pragma once

#include <JuceHeader.h>
#include "SoundTouch.h"
#include <array>
#include <atomic>
#include <vector>
#include <aubio.h>
#include <pitch/pitch.h>

#define ST_PROCESSOR_NUMBER 6
#define CHANNEL_NUMBER 1
#define SCOPE_SIZE 512

#define NO_PROCESSOR_INPUT -1
#define NO_CURRENT_FREQUENCY -2
#define NO_DESIRED_FREQUENCY -3

class ReMasteredAudioProcessor  : public AudioProcessor
{
public:
    ReMasteredAudioProcessor();
    ~ReMasteredAudioProcessor() override;
    
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    
#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif
    
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    
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
    
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::MidiKeyboardState& getKeyboardState() { return keyboardState; }
    void setVisualiserTarget(AudioVisualiserComponent* visualiserTarget) { visualiser.store(visualiserTarget); }

    void handleMidiMessage(const MidiMessage& m);

    // UI controls
    void setAutoHarmonyEnabled(bool enabled);
    bool getAutoHarmonyEnabled() const;
    void setKeyRoot(int root);
    int getKeyRoot() const;
    void setModeIndex(int mode);
    int getModeIndex() const;
    void setStyleIndex(int style);
    int getStyleIndex() const;
    void setVoiceCount(int voices);
    int getVoiceCount() const;
    void setHarmonyAmount(float amount);
    float getHarmonyAmount() const;
    void setSpreadAmount(float amount);
    float getSpreadAmount() const;
    void setDetuneCents(float cents);
    float getDetuneCents() const;
    void setGlideMs(float ms);
    float getGlideMs() const;
    void setMotionRateHz(float hz);
    float getMotionRateHz() const;
    void setMotionDepth(float amount);
    float getMotionDepth() const;
    void setDryMix(float mix);
    float getDryMix() const;
    void setOutputGain(float gain);
    float getOutputGain() const;
    void setDriveAmount(float amount);
    float getDriveAmount() const;
    void setDelayMix(float mix);
    float getDelayMix() const;
    void setDelayTimeMs(float ms);
    float getDelayTimeMs() const;
    void setDelayFeedback(float feedback);
    float getDelayFeedback() const;
    void setReverbMix(float mix);
    float getReverbMix() const;
    void setClarity(float amount);
    float getClarity() const;
    
private:
    void initSoundTouch();
    soundtouch::SoundTouch* getStProcessor(int processorIndex) const;
    void setStProcessor(int processorIndex, soundtouch::SoundTouch* newStProcessor);
    int getFirstActiveSt() const;
    int getFirstFreeSt() const;
    int getNumberOfActiveSt() const;
    int findStProcessorPlaying(float noteInHz, bool manualOnly) const;
    bool isStProcessorActive(int processorIndex) const;
    void setStProcessorActive(int processorIndex, bool active);
    void freeStProcessor(int processorIndex);
    void clearStProcessors(bool deleteProcessors);
    bool canActiveProcessorsOutput(int numSamplesToOutput) const;
    int autoTuneFreqST(float desiredFrequency, float currentFrequency, int stProcessorIndex);

    bool shouldUseAutoHarmony() const;
    void updatePitchDetection(const AudioBuffer<float>& buffer);
    void updateAutoHarmonyVoices(int numSamples);
    void applyPitchGlideAndTune(int numSamples);
    void processHarmonyVoices(const std::vector<float>& readBuffer, int numSamples, AudioBuffer<float>& outputBuffer);
    void applyPostFx(AudioBuffer<float>& outputBuffer, const std::vector<float>& dryInput);
    int quantizeMidiToScale(int midiNote) const;
    int quantizeToNearestScaleOffset(int midiOffsetFromRoot) const;
    float clampFrequency(float hz) const;
    void setReverbFromParams();
    void resetDelayBuffer();
    void syncPitchRatiosToCurrentFrequency();
    void clearAutoProcessors();
    void clearManualProcessors();

    soundtouch::SoundTouch* stProcessors[ST_PROCESSOR_NUMBER] = {};
    bool stProcessorActive[ST_PROCESSOR_NUMBER] = {};
    bool stProcessorManual[ST_PROCESSOR_NUMBER] = {};
    float stProcessorPlaying[ST_PROCESSOR_NUMBER] = {};
    float stProcessorVolume[ST_PROCESSOR_NUMBER] = {};
    float stProcessorCurrentFrequency[ST_PROCESSOR_NUMBER] = {};
    float stProcessorTargetFrequency[ST_PROCESSOR_NUMBER] = {};

    //==============================================================================
    int sampleRate = 44100;
    float currentDetectedFrequency = -1.0f;

    std::vector<std::vector<float>> st_buf;
    bool singerOnHold = false;

    int singerHoldCounter = 0;
    int manualNotesDown = 0;
    float autoMotionPhase = 0.0f;

    //==============================================================================
    int pitchDetectionWindowSize = 4096;
    aubio_pitch_t* aubioPitchDetector = nullptr;
    fvec_t* aubioInput = nullptr;
    fvec_t* aubioResult = nullptr;

    //==============================================================================
    std::atomic<AudioVisualiserComponent*> visualiser { nullptr }; // non-owning pointer set by editor
    juce::MidiKeyboardState keyboardState;

    //==============================================================================
    std::atomic<float> autoHarmonyEnabled { 0.0f };
    std::atomic<float> keyRoot { 0.0f };
    std::atomic<float> modeIndex { 5.0f };
    std::atomic<float> styleIndex { 1.0f };
    std::atomic<float> voiceCount { 4.0f };
    std::atomic<float> harmonyAmount { 0.74f };
    std::atomic<float> spreadAmount { 0.42f };
    std::atomic<float> detuneCents { 12.0f };
    std::atomic<float> glideMs { 70.0f };
    std::atomic<float> motionRateHz { 1.20f };
    std::atomic<float> motionDepth { 0.0f };
    std::atomic<float> dryMix { 0.68f };
    std::atomic<float> outputGain { 0.90f };
    std::atomic<float> driveAmount { 0.08f };
    std::atomic<float> delayMix { 0.16f };
    std::atomic<float> delayTimeMs { 320.0f };
    std::atomic<float> delayFeedback { 0.28f };
    std::atomic<float> reverbMix { 0.24f };
    std::atomic<float> clarity { 0.58f };
    std::atomic<bool> reverbDirty { true };

    std::vector<float> dryInputBuf;
    std::vector<float> wetBuf;
    std::vector<float> delayBuffer;
    int delayWriteIndex = 0;
    juce::Reverb reverb;
    juce::Reverb::Parameters reverbParams;
    float wetHighPassPrevInput = 0.0f;
    float wetHighPassPrevOutput = 0.0f;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReMasteredAudioProcessor)
};

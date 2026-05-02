#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <algorithm>
#include <cmath>
#include <limits>

namespace
{
constexpr float kMinPitchHz = 40.0f;
constexpr float kMaxPitchHz = 2000.0f;
constexpr int kNumModes = 8;
constexpr int kNumStyles = 5;

const std::array<std::array<int, 7>, kNumModes> kModeIntervals =
{{
    {{0, 2, 4, 5, 7, 9, 11}},  // Ionian
    {{0, 2, 3, 5, 7, 9, 10}},  // Dorian
    {{0, 1, 3, 5, 7, 8, 10}},  // Phrygian
    {{0, 2, 4, 6, 7, 9, 11}},  // Lydian
    {{0, 2, 4, 5, 7, 9, 10}},  // Mixolydian
    {{0, 2, 3, 5, 7, 8, 10}},  // Aeolian
    {{0, 2, 3, 5, 7, 8, 11}},  // Harmonic minor
    {{0, 1, 3, 5, 7, 8, 11}}   // Phrygian dominant
}};

const std::array<std::array<int, ST_PROCESSOR_NUMBER>, kNumStyles> kStyleIntervals =
{{
    {{0, 3, 7, 10, 12, 15}},   // Gentle
    {{0, 5, 7, 10, 12, 17}},   // Bon-ish
    {{0, 7, 12, 19, 24, 28}},  // Wide octave stack
    {{0, 2, 5, 7, 9, 12}},     // Cluster
    {{0, 4, 7, 11, 14, 19}}    // Melodic
}};

const std::array<int, 7> kScaleDegreeOffsets = {{0, 2, 4, 5, 7, 9, 11}};
} // namespace

//==============================================================================
ReMasteredAudioProcessor::ReMasteredAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::mono(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::mono(), true)
#endif
                         )
#endif
{
    initSoundTouch();

    aubioInput = new_fvec(static_cast<uint_t>(pitchDetectionWindowSize));
    aubioResult = new_fvec(1);

    setReverbFromParams();
}

ReMasteredAudioProcessor::~ReMasteredAudioProcessor()
{
    clearStProcessors(true);

    if (aubioPitchDetector != nullptr)
    {
        del_aubio_pitch(aubioPitchDetector);
        aubioPitchDetector = nullptr;
    }

    if (aubioInput != nullptr)
    {
        del_fvec(aubioInput);
        aubioInput = nullptr;
    }

    if (aubioResult != nullptr)
    {
        del_fvec(aubioResult);
        aubioResult = nullptr;
    }

    st_buf.clear();
    delayBuffer.clear();
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
    return 1;
}

int ReMasteredAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ReMasteredAudioProcessor::setCurrentProgram(int)
{
}

const juce::String ReMasteredAudioProcessor::getProgramName(int)
{
    return {};
}

void ReMasteredAudioProcessor::changeProgramName(int, const juce::String&)
{
}

//==============================================================================
void ReMasteredAudioProcessor::prepareToPlay(double newSampleRate, int samplesPerBlock)
{
    if (samplesPerBlock <= 0)
        return;

    sampleRate = static_cast<int>(newSampleRate);

    if (aubioPitchDetector != nullptr)
    {
        del_aubio_pitch(aubioPitchDetector);
        aubioPitchDetector = nullptr;
    }

    if (aubioInput != nullptr)
    {
        del_fvec(aubioInput);
        aubioInput = nullptr;
    }

    if (aubioResult != nullptr)
    {
        del_fvec(aubioResult);
        aubioResult = nullptr;
    }

    aubioInput = new_fvec(static_cast<uint_t>(samplesPerBlock));
    aubioResult = new_fvec(1);
    aubioPitchDetector = new_aubio_pitch("default",
                                         static_cast<uint_t>(pitchDetectionWindowSize),
                                         static_cast<uint_t>(samplesPerBlock),
                                         static_cast<uint_t>(sampleRate));

    initSoundTouch();
    clearStProcessors(false);

    st_buf.assign(ST_PROCESSOR_NUMBER, std::vector<float>(samplesPerBlock, 0.0f));
    dryInputBuf.assign(static_cast<size_t>(samplesPerBlock), 0.0f);
    wetBuf.assign(static_cast<size_t>(samplesPerBlock), 0.0f);

    resetDelayBuffer();
    setReverbFromParams();

    singerOnHold = false;
    singerHoldCounter = 0;
    currentDetectedFrequency = -1.0f;
    manualNotesDown = 0;
    autoMotionPhase = 0.0f;
    wetHighPassPrevInput = 0.0f;
    wetHighPassPrevOutput = 0.0f;
}

void ReMasteredAudioProcessor::releaseResources()
{
    clearStProcessors(false);
    singerOnHold = true;
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ReMasteredAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void ReMasteredAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;

    const int numInputChannels = getTotalNumInputChannels();
    const int numOutputChannels = getTotalNumOutputChannels();
    const int numSamples = buffer.getNumSamples();

    if (numSamples <= 0 || numOutputChannels <= 0)
        return;

    for (int channel = numInputChannels; channel < numOutputChannels; ++channel)
        buffer.clear(channel, 0, numSamples);

    keyboardState.processNextMidiBuffer(midiMessages, 0, numSamples, true);

    int time = 0;
    MidiMessage midiMessage;
    for (MidiBuffer::Iterator i(midiMessages); i.getNextEvent(midiMessage, time);)
        handleMidiMessage(midiMessage);

    if (numInputChannels > 0)
        std::copy(buffer.getReadPointer(0), buffer.getReadPointer(0) + numSamples, dryInputBuf.begin());
    else
        std::fill(dryInputBuf.begin(), dryInputBuf.begin() + numSamples, 0.0f);

    updatePitchDetection(buffer);

    if (shouldUseAutoHarmony())
        updateAutoHarmonyVoices(numSamples);
    else if (manualNotesDown == 0)
        clearAutoProcessors();

    applyPitchGlideAndTune(numSamples);

    buffer.clear();
    processHarmonyVoices(dryInputBuf, numSamples, buffer);
    applyPostFx(buffer, dryInputBuf);

    if (auto* uiVisualiser = visualiser.load())
        uiVisualiser->pushBuffer(buffer);
}

//==============================================================================
bool ReMasteredAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* ReMasteredAudioProcessor::createEditor()
{
    return new ReMasteredAudioProcessorEditor(*this);
}

//==============================================================================
void ReMasteredAudioProcessor::getStateInformation(MemoryBlock& destData)
{
    juce::XmlElement xml("ReMasteredState");
    xml.setAttribute("autoHarmonyEnabled", getAutoHarmonyEnabled() ? 1 : 0);
    xml.setAttribute("keyRoot", getKeyRoot());
    xml.setAttribute("modeIndex", getModeIndex());
    xml.setAttribute("styleIndex", getStyleIndex());
    xml.setAttribute("voiceCount", getVoiceCount());
    xml.setAttribute("harmonyAmount", getHarmonyAmount());
    xml.setAttribute("spreadAmount", getSpreadAmount());
    xml.setAttribute("detuneCents", getDetuneCents());
    xml.setAttribute("glideMs", getGlideMs());
    xml.setAttribute("motionRateHz", getMotionRateHz());
    xml.setAttribute("motionDepth", getMotionDepth());
    xml.setAttribute("dryMix", getDryMix());
    xml.setAttribute("outputGain", getOutputGain());
    xml.setAttribute("driveAmount", getDriveAmount());
    xml.setAttribute("delayMix", getDelayMix());
    xml.setAttribute("delayTimeMs", getDelayTimeMs());
    xml.setAttribute("delayFeedback", getDelayFeedback());
    xml.setAttribute("reverbMix", getReverbMix());
    xml.setAttribute("clarity", getClarity());
    copyXmlToBinary(xml, destData);
}

void ReMasteredAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState == nullptr || !xmlState->hasTagName("ReMasteredState"))
        return;

    setAutoHarmonyEnabled(xmlState->getIntAttribute("autoHarmonyEnabled", 0) != 0);
    setKeyRoot(xmlState->getIntAttribute("keyRoot", 0));
    setModeIndex(xmlState->getIntAttribute("modeIndex", 5));
    setStyleIndex(xmlState->getIntAttribute("styleIndex", 1));
    setVoiceCount(xmlState->getIntAttribute("voiceCount", 4));
    setHarmonyAmount(static_cast<float>(xmlState->getDoubleAttribute("harmonyAmount", 0.74)));
    setSpreadAmount(static_cast<float>(xmlState->getDoubleAttribute("spreadAmount", 0.42)));
    setDetuneCents(static_cast<float>(xmlState->getDoubleAttribute("detuneCents", 12.0)));
    setGlideMs(static_cast<float>(xmlState->getDoubleAttribute("glideMs", 70.0)));
    setMotionRateHz(static_cast<float>(xmlState->getDoubleAttribute("motionRateHz", 1.20)));
    setMotionDepth(static_cast<float>(xmlState->getDoubleAttribute("motionDepth", 0.0)));
    setDryMix(static_cast<float>(xmlState->getDoubleAttribute("dryMix", 0.68)));
    setOutputGain(static_cast<float>(xmlState->getDoubleAttribute("outputGain", 0.90)));
    setDriveAmount(static_cast<float>(xmlState->getDoubleAttribute("driveAmount", 0.08)));
    setDelayMix(static_cast<float>(xmlState->getDoubleAttribute("delayMix", 0.16)));
    setDelayTimeMs(static_cast<float>(xmlState->getDoubleAttribute("delayTimeMs", 320.0)));
    setDelayFeedback(static_cast<float>(xmlState->getDoubleAttribute("delayFeedback", 0.28)));
    setReverbMix(static_cast<float>(xmlState->getDoubleAttribute("reverbMix", 0.24)));
    setClarity(static_cast<float>(xmlState->getDoubleAttribute("clarity", 0.58)));
}

//==============================================================================
void ReMasteredAudioProcessor::initSoundTouch()
{
    for (int processorIndex = 0; processorIndex < ST_PROCESSOR_NUMBER; ++processorIndex)
    {
        if (stProcessors[processorIndex] == nullptr)
            setStProcessor(processorIndex, new soundtouch::SoundTouch());

        if (stProcessors[processorIndex] != nullptr)
        {
            stProcessors[processorIndex]->setSampleRate(sampleRate);
            stProcessors[processorIndex]->setChannels(CHANNEL_NUMBER);
            stProcessors[processorIndex]->setPitch(1.0f);
            stProcessors[processorIndex]->setSetting(SETTING_USE_QUICKSEEK, 0);
            stProcessors[processorIndex]->setSetting(SETTING_SEQUENCE_MS, 30);
            stProcessors[processorIndex]->setSetting(SETTING_SEEKWINDOW_MS, 14);
            stProcessors[processorIndex]->setSetting(SETTING_OVERLAP_MS, 8);
        }

        stProcessorActive[processorIndex] = false;
        stProcessorManual[processorIndex] = false;
        stProcessorPlaying[processorIndex] = 0.0f;
        stProcessorVolume[processorIndex] = 0.0f;
        stProcessorCurrentFrequency[processorIndex] = 0.0f;
        stProcessorTargetFrequency[processorIndex] = 0.0f;
    }
}

soundtouch::SoundTouch* ReMasteredAudioProcessor::getStProcessor(int processorIndex) const
{
    if (processorIndex < 0 || processorIndex >= ST_PROCESSOR_NUMBER)
        return nullptr;
    return stProcessors[processorIndex];
}

void ReMasteredAudioProcessor::setStProcessor(int processorIndex, soundtouch::SoundTouch* newStProcessor)
{
    if (processorIndex < 0 || processorIndex >= ST_PROCESSOR_NUMBER)
        return;
    stProcessors[processorIndex] = newStProcessor;
}

int ReMasteredAudioProcessor::getFirstActiveSt() const
{
    for (int i = 0; i < ST_PROCESSOR_NUMBER; ++i)
    {
        if (stProcessorActive[i])
            return i;
    }
    return -1;
}

int ReMasteredAudioProcessor::getFirstFreeSt() const
{
    for (int i = 0; i < ST_PROCESSOR_NUMBER; ++i)
    {
        if (!stProcessorActive[i])
            return i;
    }
    return -1;
}

int ReMasteredAudioProcessor::getNumberOfActiveSt() const
{
    int num = 0;
    for (int i = 0; i < ST_PROCESSOR_NUMBER; ++i)
    {
        if (stProcessorActive[i])
            ++num;
    }
    return num;
}

int ReMasteredAudioProcessor::findStProcessorPlaying(float noteInHz, bool manualOnly) const
{
    if (noteInHz <= 0.0f)
        return -1;

    for (int i = 0; i < ST_PROCESSOR_NUMBER; ++i)
    {
        if (!stProcessorActive[i])
            continue;
        if (manualOnly && !stProcessorManual[i])
            continue;
        if (stProcessorPlaying[i] <= 0.0f)
            continue;

        const float centsDiff = 1200.0f * std::abs(std::log2(stProcessorPlaying[i] / noteInHz));
        if (centsDiff < 9.0f)
            return i;
    }

    return -1;
}

bool ReMasteredAudioProcessor::isStProcessorActive(int processorIndex) const
{
    if (processorIndex < 0 || processorIndex >= ST_PROCESSOR_NUMBER)
        return false;
    return stProcessorActive[processorIndex];
}

void ReMasteredAudioProcessor::setStProcessorActive(int processorIndex, bool active)
{
    if (processorIndex < 0 || processorIndex >= ST_PROCESSOR_NUMBER)
        return;
    stProcessorActive[processorIndex] = active;
}

void ReMasteredAudioProcessor::freeStProcessor(int processorIndex)
{
    if (processorIndex < 0 || processorIndex >= ST_PROCESSOR_NUMBER)
        return;

    if (stProcessors[processorIndex] != nullptr)
        stProcessors[processorIndex]->clear();

    stProcessorActive[processorIndex] = false;
    stProcessorManual[processorIndex] = false;
    stProcessorPlaying[processorIndex] = 0.0f;
    stProcessorVolume[processorIndex] = 0.0f;
    stProcessorCurrentFrequency[processorIndex] = 0.0f;
    stProcessorTargetFrequency[processorIndex] = 0.0f;

    if (processorIndex < static_cast<int>(st_buf.size()))
        std::fill(st_buf[processorIndex].begin(), st_buf[processorIndex].end(), 0.0f);
}

void ReMasteredAudioProcessor::clearStProcessors(bool deleteProcessors)
{
    for (int processorIndex = 0; processorIndex < ST_PROCESSOR_NUMBER; ++processorIndex)
    {
        if (stProcessors[processorIndex] != nullptr)
        {
            stProcessors[processorIndex]->clear();
            if (deleteProcessors)
            {
                delete stProcessors[processorIndex];
                stProcessors[processorIndex] = nullptr;
            }
        }

        stProcessorActive[processorIndex] = false;
        stProcessorManual[processorIndex] = false;
        stProcessorPlaying[processorIndex] = 0.0f;
        stProcessorVolume[processorIndex] = 0.0f;
        stProcessorCurrentFrequency[processorIndex] = 0.0f;
        stProcessorTargetFrequency[processorIndex] = 0.0f;
    }
}

bool ReMasteredAudioProcessor::canActiveProcessorsOutput(int numSamplesToOutput) const
{
    bool enoughSamples = true;
    bool atLeastOneActiveProcessor = false;

    for (int processorIndex = 0; processorIndex < ST_PROCESSOR_NUMBER; ++processorIndex)
    {
        if (!stProcessorActive[processorIndex] || stProcessors[processorIndex] == nullptr)
            continue;

        if (stProcessors[processorIndex]->numSamples() < numSamplesToOutput)
            enoughSamples = false;

        atLeastOneActiveProcessor = true;
    }

    return enoughSamples && atLeastOneActiveProcessor;
}

int ReMasteredAudioProcessor::autoTuneFreqST(float desiredFrequency, float currentFrequency, int stProcessorIndex)
{
    if (currentFrequency <= 0.0f)
        return NO_CURRENT_FREQUENCY;

    if (stProcessorIndex < 0 || stProcessorIndex >= ST_PROCESSOR_NUMBER || stProcessors[stProcessorIndex] == nullptr)
        return NO_PROCESSOR_INPUT;

    if (desiredFrequency <= 0.0f)
        return NO_DESIRED_FREQUENCY;

    const float correctionFactor = juce::jlimit(0.25f, 4.0f, desiredFrequency / currentFrequency);
    stProcessors[stProcessorIndex]->setPitch(correctionFactor);
    return 1;
}

void ReMasteredAudioProcessor::updatePitchDetection(const AudioBuffer<float>& buffer)
{
    if (aubioPitchDetector == nullptr || aubioInput == nullptr || aubioResult == nullptr)
        return;

    if (buffer.getNumChannels() <= 0)
        return;

    const int numSamples = buffer.getNumSamples();
    const int aubioSamples = juce::jmin(numSamples, static_cast<int>(aubioInput->length));

    for (int i = 0; i < aubioSamples; ++i)
        fvec_set_sample(aubioInput, buffer.getSample(0, i), static_cast<uint_t>(i));

    for (int i = aubioSamples; i < static_cast<int>(aubioInput->length); ++i)
        fvec_set_sample(aubioInput, 0.0f, static_cast<uint_t>(i));

    aubio_pitch_do(aubioPitchDetector, aubioInput, aubioResult);
    const float detected = fvec_get_sample(aubioResult, 0);

    if (std::isfinite(detected) && detected >= kMinPitchHz && detected <= kMaxPitchHz)
    {
        singerHoldCounter = 0;
        singerOnHold = false;

        if (currentDetectedFrequency <= 0.0f)
            currentDetectedFrequency = detected;
        else
            currentDetectedFrequency = (currentDetectedFrequency * 0.82f) + (detected * 0.18f);

        syncPitchRatiosToCurrentFrequency();
    }
    else
    {
        ++singerHoldCounter;
        if (singerHoldCounter > 10)
            singerOnHold = true;
    }
}

bool ReMasteredAudioProcessor::shouldUseAutoHarmony() const
{
    return getAutoHarmonyEnabled() && manualNotesDown == 0 && currentDetectedFrequency >= kMinPitchHz && !singerOnHold;
}

void ReMasteredAudioProcessor::updateAutoHarmonyVoices(int numSamples)
{
    const int style = juce::jlimit(0, kNumStyles - 1, getStyleIndex());
    const int voices = juce::jlimit(1, ST_PROCESSOR_NUMBER, getVoiceCount());

    const float motionRate = juce::jlimit(0.0f, 8.0f, getMotionRateHz());
    const float motion = juce::jlimit(0.0f, 1.0f, getMotionDepth());
    const float spread = juce::jlimit(0.0f, 1.0f, getSpreadAmount());
    const float detune = juce::jlimit(0.0f, 40.0f, getDetuneCents());
    const float amount = juce::jlimit(0.0f, 1.0f, getHarmonyAmount());

    const float phaseInc = juce::MathConstants<float>::twoPi * motionRate * static_cast<float>(numSamples) / static_cast<float>(juce::jmax(1, sampleRate));
    autoMotionPhase += phaseInc;
    while (autoMotionPhase > juce::MathConstants<float>::twoPi)
        autoMotionPhase -= juce::MathConstants<float>::twoPi;

    const int baseMidi = quantizeMidiToScale(static_cast<int>(std::lround(69.0 + 12.0 * std::log2(clampFrequency(currentDetectedFrequency) / 440.0f))));

    for (int i = 0; i < ST_PROCESSOR_NUMBER; ++i)
    {
        if (i >= voices)
        {
            if (!stProcessorManual[i])
                freeStProcessor(i);
            continue;
        }

        if (stProcessors[i] == nullptr)
            continue;

        int semitoneOffset = kStyleIntervals[style][i];

        const float voiceIndexNorm = (voices > 1) ? static_cast<float>(i) / static_cast<float>(voices - 1) : 0.0f;
        semitoneOffset += static_cast<int>(std::lround((voiceIndexNorm - 0.5f) * spread * 12.0f));

        const float lfo = std::sin(autoMotionPhase + static_cast<float>(i) * 0.87f);
        semitoneOffset += static_cast<int>(std::lround(lfo * motion * 5.0f));

        if (style == 4)
        {
            const int step = static_cast<int>(std::floor(autoMotionPhase / juce::MathConstants<float>::halfPi)) % static_cast<int>(kScaleDegreeOffsets.size());
            const int melodicOffset = kScaleDegreeOffsets[(step + i) % static_cast<int>(kScaleDegreeOffsets.size())];
            semitoneOffset += melodicOffset - 5;
        }

        const int targetMidi = quantizeMidiToScale(baseMidi + semitoneOffset);

        const float signedDetune = ((i % 2) == 0 ? -1.0f : 1.0f) * detune * (0.35f + 0.65f * spread);
        const float detuneRatio = std::pow(2.0f, signedDetune / 1200.0f);
        const float targetHz = clampFrequency(MidiMessage::getMidiNoteInHertz(targetMidi) * detuneRatio);

        stProcessorActive[i] = true;
        stProcessorManual[i] = false;
        stProcessorTargetFrequency[i] = targetHz;

        if (stProcessorCurrentFrequency[i] <= 0.0f)
            stProcessorCurrentFrequency[i] = targetHz;

        stProcessorPlaying[i] = targetHz;

        const float voiceTaper = 1.0f - (voiceIndexNorm * 0.5f);
        stProcessorVolume[i] = amount * voiceTaper;
    }
}

void ReMasteredAudioProcessor::applyPitchGlideAndTune(int numSamples)
{
    if (currentDetectedFrequency < kMinPitchHz)
        return;

    const float glide = juce::jlimit(5.0f, 350.0f, getGlideMs());
    const float glideSamples = juce::jmax(1.0f, (glide / 1000.0f) * static_cast<float>(sampleRate));
    const float alpha = 1.0f - std::exp(-static_cast<float>(numSamples) / glideSamples);

    for (int i = 0; i < ST_PROCESSOR_NUMBER; ++i)
    {
        if (!stProcessorActive[i] || stProcessors[i] == nullptr)
            continue;

        if (stProcessorManual[i])
            stProcessorTargetFrequency[i] = clampFrequency(stProcessorPlaying[i]);

        if (stProcessorTargetFrequency[i] <= 0.0f)
            continue;

        if (stProcessorCurrentFrequency[i] <= 0.0f)
            stProcessorCurrentFrequency[i] = stProcessorTargetFrequency[i];

        stProcessorCurrentFrequency[i] += (stProcessorTargetFrequency[i] - stProcessorCurrentFrequency[i]) * alpha;

        autoTuneFreqST(stProcessorCurrentFrequency[i], currentDetectedFrequency, i);
    }
}

void ReMasteredAudioProcessor::processHarmonyVoices(const std::vector<float>& readBuffer, int numSamples, AudioBuffer<float>& outputBuffer)
{
    if (readBuffer.empty() || singerOnHold)
        return;

    for (int processorIndex = 0; processorIndex < ST_PROCESSOR_NUMBER; ++processorIndex)
    {
        if (!stProcessorActive[processorIndex] || stProcessors[processorIndex] == nullptr)
            continue;

        stProcessors[processorIndex]->putSamples(readBuffer.data(), numSamples);

        const int ready = stProcessors[processorIndex]->numSamples();
        if (ready < numSamples)
            continue;

        if (processorIndex >= static_cast<int>(st_buf.size()))
            continue;

        if (static_cast<int>(st_buf[processorIndex].size()) < numSamples)
            st_buf[processorIndex].resize(static_cast<size_t>(numSamples), 0.0f);

        stProcessors[processorIndex]->receiveSamples(st_buf[processorIndex].data(), numSamples);

        for (int i = 0; i < numSamples; ++i)
            outputBuffer.addSample(0, i, st_buf[processorIndex][i] * stProcessorVolume[processorIndex]);
    }

    for (int channel = 1; channel < outputBuffer.getNumChannels(); ++channel)
    {
        outputBuffer.copyFrom(channel, 0, outputBuffer, 0, 0, numSamples);
    }
}

void ReMasteredAudioProcessor::applyPostFx(AudioBuffer<float>& outputBuffer, const std::vector<float>& dryInput)
{
    const int numSamples = outputBuffer.getNumSamples();
    if (numSamples <= 0)
        return;

    if (delayBuffer.empty())
        resetDelayBuffer();

    const float dry = juce::jlimit(0.0f, 1.0f, getDryMix());
    const float gain = juce::jlimit(0.0f, 2.0f, getOutputGain());
    const float drive = juce::jlimit(0.0f, 1.0f, getDriveAmount());
    const float dMix = juce::jlimit(0.0f, 0.95f, getDelayMix());
    const float dFeedback = juce::jlimit(0.0f, 0.93f, getDelayFeedback());
    const float clean = juce::jlimit(0.0f, 1.0f, getClarity());

    // One-pole high-pass on wet signal to remove low-mid mud before ambience.
    const float hpCutoffHz = juce::jmap(clean, 90.0f, 280.0f);
    const float hpAlpha = std::exp(-juce::MathConstants<float>::twoPi * hpCutoffHz / static_cast<float>(juce::jmax(1, sampleRate)));

    const int delaySamples = juce::jlimit(1,
                                          juce::jmax(1, static_cast<int>(delayBuffer.size() - 1)),
                                          static_cast<int>((juce::jlimit(40.0f, 900.0f, getDelayTimeMs()) / 1000.0f) * static_cast<float>(sampleRate)));

    for (int i = 0; i < numSamples; ++i)
    {
        const float harmonyIn = outputBuffer.getSample(0, i);
        const float driveIn = harmonyIn * (1.0f + drive * 8.0f);
        float wet = std::tanh(driveIn);

        const int readIndex = (delayWriteIndex - delaySamples + static_cast<int>(delayBuffer.size())) % static_cast<int>(delayBuffer.size());
        const float delayed = delayBuffer[static_cast<size_t>(readIndex)];

        const float toDelay = wet + delayed * dFeedback;
        delayBuffer[static_cast<size_t>(delayWriteIndex)] = juce::jlimit(-1.0f, 1.0f, toDelay);

        delayWriteIndex = (delayWriteIndex + 1) % static_cast<int>(delayBuffer.size());

        wet = wet * (1.0f - dMix) + delayed * dMix;

        const float hpOut = hpAlpha * (wetHighPassPrevOutput + wet - wetHighPassPrevInput);
        wetHighPassPrevInput = wet;
        wetHighPassPrevOutput = hpOut;
        wet = hpOut;

        wetBuf[static_cast<size_t>(i)] = wet;
    }

    if (reverbDirty.exchange(false))
        setReverbFromParams();
    reverb.processMono(wetBuf.data(), numSamples);

    for (int i = 0; i < numSamples; ++i)
    {
        const float wet = wetBuf[static_cast<size_t>(i)];
        const float dryIn = dryInput[static_cast<size_t>(i)];
        const float mixed = ((dryIn * dry) + (wet * (1.0f - dry))) * gain;
        const float out = juce::jlimit(-1.0f, 1.0f, mixed);

        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
            outputBuffer.setSample(channel, i, out);
    }
}

int ReMasteredAudioProcessor::quantizeMidiToScale(int midiNote) const
{
    const int root = getKeyRoot();
    const int mode = juce::jlimit(0, kNumModes - 1, getModeIndex());

    int bestNote = midiNote;
    int smallestDistance = std::numeric_limits<int>::max();

    for (int candidate = midiNote - 12; candidate <= midiNote + 12; ++candidate)
    {
        int relative = candidate - root;
        int pitchClass = ((relative % 12) + 12) % 12;

        bool inScale = false;
        for (int interval : kModeIntervals[static_cast<size_t>(mode)])
        {
            if (interval == pitchClass)
            {
                inScale = true;
                break;
            }
        }

        if (!inScale)
            continue;

        const int distance = std::abs(candidate - midiNote);
        if (distance < smallestDistance)
        {
            smallestDistance = distance;
            bestNote = candidate;
        }
    }

    return juce::jlimit(24, 108, bestNote);
}

int ReMasteredAudioProcessor::quantizeToNearestScaleOffset(int midiOffsetFromRoot) const
{
    return quantizeMidiToScale(getKeyRoot() + midiOffsetFromRoot) - getKeyRoot();
}

float ReMasteredAudioProcessor::clampFrequency(float hz) const
{
    return juce::jlimit(20.0f, 5000.0f, hz);
}

void ReMasteredAudioProcessor::setReverbFromParams()
{
    const float rev = juce::jlimit(0.0f, 1.0f, getReverbMix());
    const float clean = juce::jlimit(0.0f, 1.0f, getClarity());
    reverbParams.roomSize = juce::jmap(rev, 0.22f, 0.84f);
    reverbParams.damping = juce::jmap(clean, 0.62f, 0.36f);
    reverbParams.wetLevel = juce::jmap(rev, 0.03f, 0.40f);
    reverbParams.dryLevel = 1.0f;
    reverbParams.width = 1.0f;
    reverbParams.freezeMode = 0.0f;
    reverb.setParameters(reverbParams);
}

void ReMasteredAudioProcessor::resetDelayBuffer()
{
    const int delayLength = juce::jmax(1024, sampleRate * 3);
    delayBuffer.assign(static_cast<size_t>(delayLength), 0.0f);
    delayWriteIndex = 0;
}

void ReMasteredAudioProcessor::syncPitchRatiosToCurrentFrequency()
{
    if (currentDetectedFrequency < kMinPitchHz)
        return;

    for (int i = 0; i < ST_PROCESSOR_NUMBER; ++i)
    {
        if (!stProcessorActive[i] || stProcessors[i] == nullptr)
            continue;

        const float target = stProcessorManual[i] ? stProcessorPlaying[i] : stProcessorTargetFrequency[i];
        if (target <= 0.0f)
            continue;

        autoTuneFreqST(target, currentDetectedFrequency, i);
    }
}

void ReMasteredAudioProcessor::clearAutoProcessors()
{
    for (int i = 0; i < ST_PROCESSOR_NUMBER; ++i)
    {
        if (stProcessorActive[i] && !stProcessorManual[i])
            freeStProcessor(i);
    }
}

void ReMasteredAudioProcessor::clearManualProcessors()
{
    for (int i = 0; i < ST_PROCESSOR_NUMBER; ++i)
    {
        if (stProcessorActive[i] && stProcessorManual[i])
            freeStProcessor(i);
    }
}

void ReMasteredAudioProcessor::handleMidiMessage(const MidiMessage& m)
{
    if (m.isAllNotesOff() || m.isAllSoundOff())
    {
        manualNotesDown = 0;
        clearManualProcessors();
        return;
    }

    if (m.isNoteOn())
    {
        clearAutoProcessors();
        manualNotesDown = juce::jmax(0, manualNotesDown + 1);

        const float noteHz = MidiMessage::getMidiNoteInHertz(m.getNoteNumber());

        int stIndex = getFirstFreeSt();
        if (stIndex < 0)
        {
            for (int i = 0; i < ST_PROCESSOR_NUMBER; ++i)
            {
                if (!stProcessorManual[i])
                {
                    stIndex = i;
                    break;
                }
            }
        }

        if (stIndex < 0)
            stIndex = 0;

        if (stProcessors[stIndex] == nullptr)
            return;

        stProcessorActive[stIndex] = true;
        stProcessorManual[stIndex] = true;
        stProcessorPlaying[stIndex] = noteHz;
        stProcessorTargetFrequency[stIndex] = noteHz;

        if (stProcessorCurrentFrequency[stIndex] <= 0.0f)
            stProcessorCurrentFrequency[stIndex] = noteHz;

        stProcessorVolume[stIndex] = juce::jlimit(0.10f, 1.0f, m.getFloatVelocity());

        autoTuneFreqST(noteHz, currentDetectedFrequency, stIndex);
    }
    else if (m.isNoteOff())
    {
        const float noteHz = MidiMessage::getMidiNoteInHertz(m.getNoteNumber());
        const int stIndex = findStProcessorPlaying(noteHz, true);
        freeStProcessor(stIndex);
        manualNotesDown = juce::jmax(0, manualNotesDown - 1);
    }
}

//==============================================================================
void ReMasteredAudioProcessor::setAutoHarmonyEnabled(bool enabled) { autoHarmonyEnabled.store(enabled ? 1.0f : 0.0f); }
bool ReMasteredAudioProcessor::getAutoHarmonyEnabled() const { return autoHarmonyEnabled.load() > 0.5f; }
void ReMasteredAudioProcessor::setKeyRoot(int root) { keyRoot.store(static_cast<float>(juce::jlimit(0, 11, root))); }
int ReMasteredAudioProcessor::getKeyRoot() const { return juce::jlimit(0, 11, static_cast<int>(keyRoot.load())); }
void ReMasteredAudioProcessor::setModeIndex(int mode) { modeIndex.store(static_cast<float>(juce::jlimit(0, kNumModes - 1, mode))); }
int ReMasteredAudioProcessor::getModeIndex() const { return juce::jlimit(0, kNumModes - 1, static_cast<int>(modeIndex.load())); }
void ReMasteredAudioProcessor::setStyleIndex(int style) { styleIndex.store(static_cast<float>(juce::jlimit(0, kNumStyles - 1, style))); }
int ReMasteredAudioProcessor::getStyleIndex() const { return juce::jlimit(0, kNumStyles - 1, static_cast<int>(styleIndex.load())); }
void ReMasteredAudioProcessor::setVoiceCount(int voices) { voiceCount.store(static_cast<float>(juce::jlimit(1, ST_PROCESSOR_NUMBER, voices))); }
int ReMasteredAudioProcessor::getVoiceCount() const { return juce::jlimit(1, ST_PROCESSOR_NUMBER, static_cast<int>(voiceCount.load())); }
void ReMasteredAudioProcessor::setHarmonyAmount(float amount) { harmonyAmount.store(juce::jlimit(0.0f, 1.0f, amount)); }
float ReMasteredAudioProcessor::getHarmonyAmount() const { return juce::jlimit(0.0f, 1.0f, harmonyAmount.load()); }
void ReMasteredAudioProcessor::setSpreadAmount(float amount) { spreadAmount.store(juce::jlimit(0.0f, 1.0f, amount)); }
float ReMasteredAudioProcessor::getSpreadAmount() const { return juce::jlimit(0.0f, 1.0f, spreadAmount.load()); }
void ReMasteredAudioProcessor::setDetuneCents(float cents) { detuneCents.store(juce::jlimit(0.0f, 40.0f, cents)); }
float ReMasteredAudioProcessor::getDetuneCents() const { return juce::jlimit(0.0f, 40.0f, detuneCents.load()); }
void ReMasteredAudioProcessor::setGlideMs(float ms) { glideMs.store(juce::jlimit(5.0f, 350.0f, ms)); }
float ReMasteredAudioProcessor::getGlideMs() const { return juce::jlimit(5.0f, 350.0f, glideMs.load()); }
void ReMasteredAudioProcessor::setMotionRateHz(float hz) { motionRateHz.store(juce::jlimit(0.0f, 8.0f, hz)); }
float ReMasteredAudioProcessor::getMotionRateHz() const { return juce::jlimit(0.0f, 8.0f, motionRateHz.load()); }
void ReMasteredAudioProcessor::setMotionDepth(float amount) { motionDepth.store(juce::jlimit(0.0f, 1.0f, amount)); }
float ReMasteredAudioProcessor::getMotionDepth() const { return juce::jlimit(0.0f, 1.0f, motionDepth.load()); }
void ReMasteredAudioProcessor::setDryMix(float mix) { dryMix.store(juce::jlimit(0.0f, 1.0f, mix)); }
float ReMasteredAudioProcessor::getDryMix() const { return juce::jlimit(0.0f, 1.0f, dryMix.load()); }
void ReMasteredAudioProcessor::setOutputGain(float gain) { outputGain.store(juce::jlimit(0.0f, 2.0f, gain)); }
float ReMasteredAudioProcessor::getOutputGain() const { return juce::jlimit(0.0f, 2.0f, outputGain.load()); }
void ReMasteredAudioProcessor::setDriveAmount(float amount) { driveAmount.store(juce::jlimit(0.0f, 1.0f, amount)); }
float ReMasteredAudioProcessor::getDriveAmount() const { return juce::jlimit(0.0f, 1.0f, driveAmount.load()); }
void ReMasteredAudioProcessor::setDelayMix(float mix) { delayMix.store(juce::jlimit(0.0f, 0.95f, mix)); }
float ReMasteredAudioProcessor::getDelayMix() const { return juce::jlimit(0.0f, 0.95f, delayMix.load()); }
void ReMasteredAudioProcessor::setDelayTimeMs(float ms) { delayTimeMs.store(juce::jlimit(40.0f, 900.0f, ms)); }
float ReMasteredAudioProcessor::getDelayTimeMs() const { return juce::jlimit(40.0f, 900.0f, delayTimeMs.load()); }
void ReMasteredAudioProcessor::setDelayFeedback(float feedback) { delayFeedback.store(juce::jlimit(0.0f, 0.93f, feedback)); }
float ReMasteredAudioProcessor::getDelayFeedback() const { return juce::jlimit(0.0f, 0.93f, delayFeedback.load()); }
void ReMasteredAudioProcessor::setReverbMix(float mix) { reverbMix.store(juce::jlimit(0.0f, 1.0f, mix)); reverbDirty.store(true); }
float ReMasteredAudioProcessor::getReverbMix() const { return juce::jlimit(0.0f, 1.0f, reverbMix.load()); }
void ReMasteredAudioProcessor::setClarity(float amount) { clarity.store(juce::jlimit(0.0f, 1.0f, amount)); reverbDirty.store(true); }
float ReMasteredAudioProcessor::getClarity() const { return juce::jlimit(0.0f, 1.0f, clarity.load()); }

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReMasteredAudioProcessor();
}

#ifdef _MSC_VER
#pragma warning(disable: 4702 4244 4305 4458 4100)
#endif

#include "PluginProcessor.h"

#include "../audio_engine/AudioLib/ValueTables.h"
#include "../audio_engine/FastSin.h"
#include "PluginEditor.h"
#include <cmath>
#include <cstdint>

#ifdef _MSC_VER
#include <intrin.h>
#endif
juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

namespace
{
    bool shouldSmoothParameter(Parameter parameter)
    {
        switch (parameter)
        {
        case Parameter::InputMix:
        case Parameter::PreDelay:
        case Parameter::TapLength:
        case Parameter::TapGain:
        case Parameter::TapDecay:
        case Parameter::DiffusionDelay:
        case Parameter::DiffusionFeedback:
        case Parameter::LineDelay:
        case Parameter::LineDecay:
        case Parameter::LateDiffusionDelay:
        case Parameter::LateDiffusionFeedback:
        case Parameter::PostLowShelfGain:
        case Parameter::PostLowShelfFrequency:
        case Parameter::PostHighShelfGain:
        case Parameter::PostHighShelfFrequency:
        case Parameter::PostCutoffFrequency:
        case Parameter::EarlyDiffusionModAmount:
        case Parameter::EarlyDiffusionModRate:
        case Parameter::LineModAmount:
        case Parameter::LineModRate:
        case Parameter::LateDiffusionModAmount:
        case Parameter::LateDiffusionModRate:
        case Parameter::CrossSeed:
        case Parameter::DryOut:
        case Parameter::PredelayOut:
        case Parameter::EarlyOut:
        case Parameter::MainOut:
        case Parameter::WidthOut:
            return true;

        default:
            return false;
        }
    }

    double getSmoothingTimeSeconds(Parameter parameter)
    {
        switch (parameter)
        {
        case Parameter::PreDelay:
        case Parameter::TapLength:
        case Parameter::DiffusionDelay:
        case Parameter::LineDelay:
        case Parameter::LateDiffusionDelay:
            return 0.045; // delay-time moves are the highest click risk

        case Parameter::LineDecay:
        case Parameter::DiffusionFeedback:
        case Parameter::LateDiffusionFeedback:
            return 0.035;

        case Parameter::DryOut:
        case Parameter::PredelayOut:
        case Parameter::EarlyOut:
        case Parameter::MainOut:
        case Parameter::WidthOut:
        case Parameter::InputMix:
            return 0.020;

        default:
            return 0.025;
        }
    }
}

AudioPluginAudioProcessor::CpuPath AudioPluginAudioProcessor::detectCpuPath()
{
#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_IX86))
    int regs[4] = {0, 0, 0, 0};
    __cpuidex(regs, 0, 0);
    const int maxLeaf = regs[0];

    if (maxLeaf >= 1)
    {
        __cpuidex(regs, 1, 0);
        const bool hasSSE2 = (regs[3] & (1 << 26)) != 0;
        const bool hasOSXSAVE = (regs[2] & (1 << 27)) != 0;
        const bool hasAVX = (regs[2] & (1 << 28)) != 0;

        if (hasOSXSAVE && hasAVX && maxLeaf >= 7)
        {
            const auto xcr0 = _xgetbv(0);
            const bool osSavesYMM = (xcr0 & 0x6) == 0x6;

            if (osSavesYMM)
            {
                __cpuidex(regs, 7, 0);
                const bool hasAVX2 = (regs[1] & (1 << 5)) != 0;
                if (hasAVX2)
                    return CpuPath::AVX2;
            }
        }

        if (hasSSE2)
            return CpuPath::SSE2;
    }
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
    __builtin_cpu_init();
    if (__builtin_cpu_supports("avx2"))
        return CpuPath::AVX2;
    if (__builtin_cpu_supports("sse2"))
        return CpuPath::SSE2;
#endif

    return CpuPath::Scalar;
}

const char *AudioPluginAudioProcessor::getCpuPathName(CpuPath path)
{
    switch (path)
    {
    case CpuPath::AVX2:
        return "AVX2";
    case CpuPath::SSE2:
        return "SSE2";
    default:
        return "Scalar";
    }
}

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
                         ),
      treeState(*this, &undoManager, juce::Identifier("CloudReverb"), createParameterLayout()),
      reverb(48000)
{
    AudioLib::ValueTables::Init();
    CloudSeed::FastSin::Init();

    cpuPath = detectCpuPath();
    DBG("CloudReverb CPU path detected: " << getCpuPathName(cpuPath));

    reverb.ClearBuffers(); // clear buffers before we start do dsp stuff.

    for (auto param : getParameters())
    {
        if (auto paramWithID = dynamic_cast<juce::AudioProcessorParameterWithID *>(param))
        {
            treeState.addParameterListener(paramWithID->paramID, this);
            if (auto rangedParam = dynamic_cast<juce::RangedAudioParameter *>(param))
            {
                paramCache[paramWithID->paramID] = rangedParam;
            }
        }
    }

    map.insert({"InputMix", Parameter::InputMix});
    map.insert({"PreDelay", Parameter::PreDelay});
    map.insert({"HighPass", Parameter::HighPass});
    map.insert({"LowPass", Parameter::LowPass});
    map.insert({"TapEnabled", Parameter::TapEnabled});
    map.insert({"TapCount", Parameter::TapCount});
    map.insert({"TapLength", Parameter::TapLength});
    map.insert({"TapGain", Parameter::TapGain});
    map.insert({"TapDecay", Parameter::TapDecay});
    map.insert({"DiffusionEnabled", Parameter::DiffusionEnabled});
    map.insert({"DiffusionStages", Parameter::DiffusionStages});
    map.insert({"DiffusionDelay", Parameter::DiffusionDelay});
    map.insert({"DiffusionFeedback", Parameter::DiffusionFeedback});
    map.insert({"LateDelayEnabled", Parameter::LateDelayEnabled});
    map.insert({"LineCount", Parameter::LineCount});
    map.insert({"LineDelay", Parameter::LineDelay});
    map.insert({"LineDecay", Parameter::LineDecay});
    map.insert({"LateDiffusionEnabled", Parameter::LateDiffusionEnabled});
    map.insert({"LateDiffusionStages", Parameter::LateDiffusionStages});
    map.insert({"LateDiffusionDelay", Parameter::LateDiffusionDelay});
    map.insert({"LateDiffusionFeedback", Parameter::LateDiffusionFeedback});
    map.insert({"PostLowShelfGain", Parameter::PostLowShelfGain});
    map.insert({"PostLowShelfFrequency", Parameter::PostLowShelfFrequency});
    map.insert({"PostHighShelfGain", Parameter::PostHighShelfGain});
    map.insert({"PostHighShelfFrequency", Parameter::PostHighShelfFrequency});
    map.insert({"PostCutoffFrequency", Parameter::PostCutoffFrequency});
    map.insert({"EarlyDiffusionModAmount", Parameter::EarlyDiffusionModAmount});
    map.insert({"EarlyDiffusionModRate", Parameter::EarlyDiffusionModRate});
    map.insert({"LineModAmount", Parameter::LineModAmount});
    map.insert({"LineModRate", Parameter::LineModRate});
    map.insert({"LateDiffusionModAmount", Parameter::LateDiffusionModAmount});
    map.insert({"LateDiffusionModRate", Parameter::LateDiffusionModRate});
    map.insert({"TapSeed", Parameter::TapSeed});
    map.insert({"DiffusionSeed", Parameter::DiffusionSeed});
    map.insert({"DelaySeed", Parameter::DelaySeed});
    map.insert({"PostDiffusionSeed", Parameter::PostDiffusionSeed});
    map.insert({"CrossSeed", Parameter::CrossSeed});
    map.insert({"DryOut", Parameter::DryOut});
    map.insert({"PredelayOut", Parameter::PredelayOut});
    map.insert({"EarlyOut", Parameter::EarlyOut});
    map.insert({"MainOut", Parameter::MainOut});
    map.insert({"Width", Parameter::WidthOut});
    map.insert({"HiPassEnabled", Parameter::HiPassEnabled});
    map.insert({"LowPassEnabled", Parameter::LowPassEnabled});
    map.insert({"LowShelfEnabled", Parameter::LowShelfEnabled});
    map.insert({"HighShelfEnabled", Parameter::HighShelfEnabled});
    map.insert({"CutoffEnabled", Parameter::CutoffEnabled});
    map.insert({"LateStageTap", Parameter::LateStageTap});
    map.insert({"Interpolation", Parameter::Interpolation});

    // https://github.com/ValdemarOrn/CloudSeed/blob/master/Factory%20Programs/Chorus%20Delay.json
    setPreset(cloudPresets::ChorusDelay);

    for (auto &pair : paramCache)
    {
        parameterChanged(pair.first, pair.second->convertFrom0to1(pair.second->getValue()));
    }
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor() {}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const { return JucePlugin_Name; }

bool AudioPluginAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool AudioPluginAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram() { return 0; }

void AudioPluginAudioProcessor::setCurrentProgram(int index) { juce::ignoreUnused(index); }

const juce::String AudioPluginAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName(int index, const juce::String &newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused(samplesPerBlock);
    reverb.prepare(sampleRate, samplesPerBlock);
    reverb.SetSamplerate(sampleRate);
    presetTransitionRequested.store(false, std::memory_order_release);
    presetFadeInSamplesRemaining = 0;
    presetFadeInSamplesTotal = 0;
    smartIdleActive = false;
    smartIdleSilentSamples = 0;
}

void AudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}

void AudioPluginAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                             juce::MidiBuffer &midiMessages)
{
    juce::ignoreUnused(midiMessages);

    if (is_bypassed)
    {
        DBG("Processing resumed");
        is_bypassed = false;
    }

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // we only process stero signal, so we clear unused output buffers.
    for (auto i = 2; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    if (totalNumInputChannels == 1 && totalNumOutputChannels == 2)
        buffer.clear(1, 0, buffer.getNumSamples());

    const auto numSamples = buffer.getNumSamples();
    constexpr float sleepInputThreshold = 1.0e-7f;
    constexpr float sleepOutputThreshold = 1.0e-6f; // about -120 dBFS, faster tail-safe sleep
    constexpr float wakeInputThreshold = 3.0e-7f;
    constexpr double smartIdleHoldSeconds = 0.75;   // shorter hold after tail is inaudible

    float inputPeak = 0.0f;
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
        inputPeak = juce::jmax(inputPeak, buffer.getMagnitude(channel, 0, numSamples));

    if (presetTransitionRequested.exchange(false, std::memory_order_acq_rel))
    {
        reverb.ClearBuffers();
        const auto fadeMs = juce::jlimit(5, 100, presetTransitionFadeMs.exchange(50, std::memory_order_acq_rel));
        presetFadeInSamplesTotal = juce::jmax(32, (int)(getSampleRate() * (fadeMs / 1000.0)));
        presetFadeInSamplesRemaining = presetFadeInSamplesTotal;
    }

    bool immediateDirty[parameterCount] = { false };
    Message immediateParams[parameterCount];

    Message message;
    while (queue.try_dequeue(message))
    {
        const int pIndex = static_cast<int>(message.param);
        if (pIndex < 0 || pIndex >= parameterCount)
            continue;

        targetParams[pIndex] = message;

        if (!shouldSmoothParameter(message.param))
        {
            smoothedParams[pIndex] = message;
            smootherInitialised[pIndex] = true;
            smoothingSamplesRemaining[pIndex] = 0;
            immediateParams[pIndex] = message;
            immediateDirty[pIndex] = true;
            continue;
        }

        if (!smootherInitialised[pIndex])
        {
            // First boot/restore value: apply immediately so the default preset is exact.
            smoothedParams[pIndex] = message;
            smootherInitialised[pIndex] = true;
            smoothingSamplesRemaining[pIndex] = 0;
            immediateParams[pIndex] = message;
            immediateDirty[pIndex] = true;
        }
        else
        {
            const auto smoothingSamples = juce::jmax(1, (int)std::round(getSampleRate() * getSmoothingTimeSeconds(message.param)));
            smoothingSamplesRemaining[pIndex] = smoothingSamples;
        }
    }

    for (int i = 0; i < parameterCount; ++i)
    {
        if (immediateDirty[i])
            reverb.updateParameter(immediateParams[i].param,
                                   immediateParams[i].newScaledValue,
                                   immediateParams[i].newNormalisedValue);
    }

    const auto blockSamples = buffer.getNumSamples();
    bool parameterSmoothingActive = false;
    for (int i = 0; i < parameterCount; ++i)
    {
        auto remaining = smoothingSamplesRemaining[i];
        if (remaining <= 0)
            continue;

        parameterSmoothingActive = true;
        const auto samplesThisStep = juce::jmin(blockSamples, remaining);
        const auto alpha = (float)samplesThisStep / (float)remaining;

        auto &current = smoothedParams[i];
        const auto &target = targetParams[i];
        current.param = target.param;
        current.newScaledValue += (target.newScaledValue - current.newScaledValue) * alpha;
        current.newNormalisedValue += (target.newNormalisedValue - current.newNormalisedValue) * alpha;

        remaining -= samplesThisStep;
        if (remaining <= 0)
        {
            current = target; // exact final value, preserving the original settled reverb tone
            remaining = 0;
        }

        smoothingSamplesRemaining[i] = remaining;
        reverb.updateParameter(current.param, current.newScaledValue, current.newNormalisedValue);
    }

    if (smartIdleActive)
    {
        if (inputPeak < wakeInputThreshold)
        {
            buffer.clear();
            return;
        }

        smartIdleActive = false;
        smartIdleSilentSamples = 0;
    }

    // real dsp
    const float *const *in_sig = buffer.getArrayOfReadPointers();
    float *const *out_sig = buffer.getArrayOfWritePointers();
    if (totalNumInputChannels == 2 && totalNumOutputChannels == 2)
    {
        reverb.Process(in_sig, out_sig, numSamples);
    }
    else if (totalNumInputChannels >= 1 && totalNumOutputChannels >= 1)
    {
        reverb.ProcessMono(in_sig, out_sig, numSamples);
    }

    if (presetFadeInSamplesRemaining > 0 && presetFadeInSamplesTotal > 0)
    {
        const auto samples = numSamples;
        for (int sample = 0; sample < samples && presetFadeInSamplesRemaining > 0; ++sample)
        {
            const auto gain = 1.0f - (float)presetFadeInSamplesRemaining / (float)presetFadeInSamplesTotal;
            for (int channel = 0; channel < totalNumOutputChannels; ++channel)
                buffer.setSample(channel, sample, buffer.getSample(channel, sample) * gain);
            --presetFadeInSamplesRemaining;
        }
    }

    float outputPeak = 0.0f;
    for (int channel = 0; channel < totalNumOutputChannels; ++channel)
        outputPeak = juce::jmax(outputPeak, buffer.getMagnitude(channel, 0, numSamples));

    if (inputPeak < sleepInputThreshold
        && outputPeak < sleepOutputThreshold
        && !parameterSmoothingActive
        && presetFadeInSamplesRemaining <= 0)
    {
        smartIdleSilentSamples += numSamples;
        const auto requiredSilentSamples = juce::jmax(1, (int)(getSampleRate() * smartIdleHoldSeconds));

        if (smartIdleSilentSamples >= requiredSilentSamples)
        {
            // Safe point: both input and output tail are below the inaudible threshold
            // for a sustained period. Clearing now should not be audible.
            smartIdleActive = true;
            smartIdleSilentSamples = requiredSilentSamples;
            reverb.ClearBuffers();
            buffer.clear();
        }
    }
    else
    {
        smartIdleSilentSamples = 0;
    }
}

void AudioPluginAudioProcessor::processBlockBypassed(juce::AudioBuffer<float> &buffer,
                                                     juce::MidiBuffer &midiMessages)
{
    if (!is_bypassed)
    {
        DBG("Processing bypassed");
        is_bypassed = true;
        reverb.ClearBuffers();
    }
    juce::AudioProcessor::processBlockBypassed(buffer, midiMessages);
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor(*this);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto tree_state = treeState.copyState();
    std::unique_ptr<juce::XmlElement> xml(tree_state.createXml());
    struct ui_state state = this->state.get_state();

    auto *ui_state = xml->getChildByName("UIState");

    if (ui_state == nullptr)
    {
        ui_state = xml->createNewChildElement("UIState");
    }

    ui_state->setAttribute("presetID", state.preset_id);
    ui_state->setAttribute("width", state.width);
    ui_state->setAttribute("height", state.height);
    ui_state->setAttribute("presetName", currentPresetDisplayName);
    DBG("store state");
    // std::cout << xml->toString();
    copyXmlToBinary(*xml, destData);
}

void AudioPluginAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    auto *ui_state = xmlState->getChildByName("UIState");
    if (ui_state != nullptr)
    {
        // selectedPresetId = ui_state->getIntAttribute("presetID");
        struct ui_state state = {
            ui_state->getIntAttribute("width", 0),
            ui_state->getIntAttribute("height", 0),
            ui_state->getIntAttribute("presetID", 1),
        };
        this->state.set_state(&state);
        currentPresetDisplayName = ui_state->getStringAttribute("presetName", {});
    }
    DBG("restore state");
    // std::cout << xmlState->toString();

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(treeState.state.getType()))
            treeState.replaceState(juce::ValueTree::fromXml(*xmlState));
    triggerAsyncUpdate();
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() { return new AudioPluginAudioProcessor(); }

#define FLOAT_TO_TEXT(numberOfDecimalPlaces, SUFFIX)                                               \
    [](float value, int) { return juce::String(value, numberOfDecimalPlaces) + " " + #SUFFIX; }

#define INT_TO_TEXT(SUFFIX, EMPTY_TOKEN)                                                           \
    [](float value, int) { return juce::String((int)value) + " " + #SUFFIX; }

#define TO_TEXT_IN_DB(numberOfDecimalPlaces)                                                       \
    [](float value, int) {                                                                         \
        return value == 0 ? "0"                                                                    \
                          : juce::String(20 * log10(value), numberOfDecimalPlaces) + " " + "dB";   \
    }

#define MAKE_NORMALISABLE_RANGE_FLOAT(START, END, P)                                               \
    juce::NormalisableRange<float>(                                                                \
        START, END,                                                                                \
        [](float start, float end, float value) {                                                  \
            return start + (std::pow(P, value) - 1) / (P - 1) * (end - start);                     \
        },                                                                                         \
        [](float start, float end, float value) {                                                  \
            return std::log((value - start) / (end - start) * (P - 1) + 1) / std::log(P);          \
        })

#define MAKE_PARAMETER_FLOAT(NAME, START, END, DEFAULT_VALUE, P, SUFFIX)                           \
    std::make_unique<juce::AudioParameterFloat>(                                                   \
        #NAME, #NAME, MAKE_NORMALISABLE_RANGE_FLOAT(START, END, P), DEFAULT_VALUE, juce::String(), \
        juce::AudioProcessorParameter::genericParameter, FLOAT_TO_TEXT(2, SUFFIX))

#define MAKE_PARAMETER_DB_FLOAT(NAME, START, END, DEFAULT_VALUE, P)                                \
    std::make_unique<juce::AudioParameterFloat>(                                                   \
        #NAME, #NAME, MAKE_NORMALISABLE_RANGE_FLOAT(START, END, P), DEFAULT_VALUE, juce::String(), \
        juce::AudioProcessorParameter::genericParameter, TO_TEXT_IN_DB(2))

#define MAKE_PARAMETER_LINEAR_FLOAT(NAME, START, END, DEFAULT_VALUE, SUFFIX)                       \
    std::make_unique<juce::AudioParameterFloat>(                                                   \
        #NAME, #NAME, juce::NormalisableRange<float>(START, END), DEFAULT_VALUE, juce::String(),   \
        juce::AudioProcessorParameter::genericParameter, FLOAT_TO_TEXT(2, SUFFIX))

#define MAKE_PARAMETER_INT(NAME, START, END, DEFAULT_VALUE, SUFFIX)                                \
    std::make_unique<juce::AudioParameterInt>(#NAME, #NAME, START, END, DEFAULT_VALUE,             \
                                              juce::String(), INT_TO_TEXT(SUFFIX, ))

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    std::vector<std::unique_ptr<juce::AudioProcessorParameterGroup>> params;

    auto InputMix = MAKE_PARAMETER_LINEAR_FLOAT(InputMix, 0.f, 1.f, 0.f, );

    auto PreDelay = MAKE_PARAMETER_INT(PreDelay, 0, 1000, 0, ms);

    auto HighPass = MAKE_PARAMETER_FLOAT(HighPass, 20.0f, 1000.0f, 20.0f, 16, Hz);

    auto LowPass = MAKE_PARAMETER_FLOAT(LowPass, 400.0f, 20000.0f, 20000.0f, 16, Hz);

    auto TapEnabled =
        std::make_unique<juce::AudioParameterBool>("TapEnabled", "TapEnabled", true);

    auto TapCount = MAKE_PARAMETER_INT(TapCount, 1, 50, 1, taps);

    auto TapLength = MAKE_PARAMETER_INT(TapLength, 0, 500, 0, ms);

    auto TapGain = MAKE_PARAMETER_FLOAT(TapGain, 0.0f, 1.0f, 0.5f, 100, );

    auto TapDecay = MAKE_PARAMETER_LINEAR_FLOAT(TapDecay, 0.f, 1.f, 0.f, );

    auto DiffusionEnabled =
        std::make_unique<juce::AudioParameterBool>("DiffusionEnabled", "DiffusionEnabled", false);

    auto DiffusionStages = MAKE_PARAMETER_INT(DiffusionStages, 1, 8, 1, stages);

    auto DiffusionDelay = MAKE_PARAMETER_INT(DiffusionDelay, 10, 100, 10, ms);

    auto DiffusionFeedback = MAKE_PARAMETER_LINEAR_FLOAT(DiffusionFeedback, 0.f, 1.f, 0.f, );

    auto LateDelayEnabled =
        std::make_unique<juce::AudioParameterBool>("LateDelayEnabled", "LateDelayEnabled", true);

    auto LineCount = MAKE_PARAMETER_INT(LineCount, 1, 12, 1, parallel delay);

    auto LineDelay = std::make_unique<juce::AudioParameterFloat>(
        "LineDelay", "LateDelay", MAKE_NORMALISABLE_RANGE_FLOAT(20.0f, 1000.0f, 100),
        20.f, juce::String(), juce::AudioProcessorParameter::genericParameter,
        FLOAT_TO_TEXT(2, ms));

    auto LineDecay = std::make_unique<juce::AudioParameterFloat>(
        "LineDecay", "LateDecay", MAKE_NORMALISABLE_RANGE_FLOAT(0.05f, 60.f, 1000),
        1.f, juce::String(), juce::AudioProcessorParameter::genericParameter,
        FLOAT_TO_TEXT(2, s));

    auto LateDiffusionEnabled = std::make_unique<juce::AudioParameterBool>(
        "LateDiffusionEnabled", "LateDiffusionEnabled", false);

    auto LateDiffusionStages = MAKE_PARAMETER_INT(LateDiffusionStages, 1, 8, 1, stages);

    auto LateDiffusionDelay = MAKE_PARAMETER_INT(LateDiffusionDelay, 10.0f, 100.0f, 10.f, ms);

    auto LateDiffusionFeedback =
        MAKE_PARAMETER_LINEAR_FLOAT(LateDiffusionFeedback, 0.f, 1.f, 0.5f, );

    auto PostLowShelfGain = MAKE_PARAMETER_FLOAT(PostLowShelfGain, 0.f, 1.f, 0.5f, 100, );

    auto PostLowShelfFrequency =
        MAKE_PARAMETER_FLOAT(PostLowShelfFrequency, 20.f, 1000.f, 1000.f, 16, Hz);

    auto PostHighShelfGain = MAKE_PARAMETER_FLOAT(PostHighShelfGain, 0.f, 1.f, 0.5f, 100, );

    auto PostHighShelfFrequency =
        MAKE_PARAMETER_FLOAT(PostHighShelfFrequency, 400.f, 20000.f, 20000.f, 16, Hz);

    auto PostCutoffFrequency =
        MAKE_PARAMETER_FLOAT(PostCutoffFrequency, 400.f, 20000.f, 20000.f, 16, Hz);

    auto EarlyDiffusionModAmount =
        MAKE_PARAMETER_LINEAR_FLOAT(EarlyDiffusionModAmount, 0.f, 2.5f, 0.f, );

    auto EarlyDiffusionModRate =
        MAKE_PARAMETER_FLOAT(EarlyDiffusionModRate, 0.f, 5.f, 0.5f, 100, Hz);

    auto LineModAmount = std::make_unique<juce::AudioParameterFloat>(
        "LineModAmount", "LateModAmount", juce::NormalisableRange<float>(0.f, 2.5f),
        0.f, juce::String(), juce::AudioProcessorParameter::genericParameter,
        FLOAT_TO_TEXT(2, ));

    auto LineModRate = std::make_unique<juce::AudioParameterFloat>(
        "LineModRate", "LateModRate", MAKE_NORMALISABLE_RANGE_FLOAT(0.f, 5.f, 100),
        0.5f, juce::String(), juce::AudioProcessorParameter::genericParameter,
        FLOAT_TO_TEXT(2, Hz));

    auto LateDiffusionModAmount =
        MAKE_PARAMETER_LINEAR_FLOAT(LateDiffusionModAmount, 0.f, 2.5f, 0.f, );

    auto LateDiffusionModRate = MAKE_PARAMETER_FLOAT(LateDiffusionModRate, 0.f, 5.f, 0.5f, 100, Hz);

    auto TapSeed = MAKE_PARAMETER_INT(TapSeed, 1, 1000000, 1, );

    auto DiffusionSeed = MAKE_PARAMETER_INT(DiffusionSeed, 1, 1000000, 1, );

    auto DelaySeed = MAKE_PARAMETER_INT(DelaySeed, 1, 1000000, 1, );

    auto PostDiffusionSeed = MAKE_PARAMETER_INT(PostDiffusionSeed, 1, 1000000, 1, );

    auto CrossSeed = MAKE_PARAMETER_LINEAR_FLOAT(CrossSeed, 0.f, 1.f, 0.f, );

    auto DryOut = MAKE_PARAMETER_DB_FLOAT(DryOut, 0.f, 1.f, 1.f, 10);

    auto PredelayOut = MAKE_PARAMETER_DB_FLOAT(PredelayOut, 0.f, 1.f, 0.f, 10);
    auto EarlyOut = MAKE_PARAMETER_DB_FLOAT(EarlyOut, 0.f, 1.f, 0.f, 10);

    auto MainOut = MAKE_PARAMETER_DB_FLOAT(MainOut, 0.f, 1.f, 0.f, 10);

    // Stereo width: 0.0 = mono, 1.0 = full original stereo width, 2.0 = extra-wide.
    auto Width = MAKE_PARAMETER_LINEAR_FLOAT(Width, 0.f, 2.f, 1.0f, );

    auto HiPassEnabled =
        std::make_unique<juce::AudioParameterBool>("HiPassEnabled", "HiPassEnabled", false);

    auto LowPassEnabled =
        std::make_unique<juce::AudioParameterBool>("LowPassEnabled", "LowPassEnabled", false);

    auto LowShelfEnabled =
        std::make_unique<juce::AudioParameterBool>("LowShelfEnabled", "LowShelfEnabled", false);

    auto HighShelfEnabled =
        std::make_unique<juce::AudioParameterBool>("HighShelfEnabled", "HighShelfEnabled", false);

    auto CutoffEnabled =
        std::make_unique<juce::AudioParameterBool>("CutoffEnabled", "CutoffEnabled", false);

    auto LateStageTap = std::make_unique<juce::AudioParameterFloat>(
        "LateStageTap", "LateStageTap", juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f);

    auto Interpolation =
        std::make_unique<juce::AudioParameterBool>("Interpolation", "Interpolation", false);

    auto group =
        std::make_unique<juce::AudioProcessorParameterGroup>("CloudReverb_ID", "CloudReverb", "|");

    group->addChild(std::move(InputMix));
    group->addChild(std::move(PreDelay));
    group->addChild(std::move(HighPass));
    group->addChild(std::move(LowPass));
    group->addChild(std::move(TapEnabled));
    group->addChild(std::move(TapCount));
    group->addChild(std::move(TapLength));
    group->addChild(std::move(TapGain));
    group->addChild(std::move(TapDecay));
    group->addChild(std::move(DiffusionEnabled));
    group->addChild(std::move(DiffusionStages));
    group->addChild(std::move(DiffusionDelay));
    group->addChild(std::move(DiffusionFeedback));
    group->addChild(std::move(LateDelayEnabled));
    group->addChild(std::move(LineCount));
    group->addChild(std::move(LineDelay));
    group->addChild(std::move(LineDecay));
    group->addChild(std::move(LateDiffusionEnabled));
    group->addChild(std::move(LateDiffusionStages));
    group->addChild(std::move(LateDiffusionDelay));
    group->addChild(std::move(LateDiffusionFeedback));
    group->addChild(std::move(PostLowShelfGain));
    group->addChild(std::move(PostLowShelfFrequency));
    group->addChild(std::move(PostHighShelfGain));
    group->addChild(std::move(PostHighShelfFrequency));
    group->addChild(std::move(PostCutoffFrequency));
    group->addChild(std::move(EarlyDiffusionModAmount));
    group->addChild(std::move(EarlyDiffusionModRate));
    group->addChild(std::move(LineModAmount));
    group->addChild(std::move(LineModRate));
    group->addChild(std::move(LateDiffusionModAmount));
    group->addChild(std::move(LateDiffusionModRate));
    group->addChild(std::move(TapSeed));
    group->addChild(std::move(DiffusionSeed));
    group->addChild(std::move(DelaySeed));
    group->addChild(std::move(PostDiffusionSeed));
    group->addChild(std::move(CrossSeed));
    group->addChild(std::move(DryOut));
    group->addChild(std::move(PredelayOut));
    group->addChild(std::move(EarlyOut));
    group->addChild(std::move(MainOut));
    group->addChild(std::move(Width));
    group->addChild(std::move(HiPassEnabled));
    group->addChild(std::move(LowPassEnabled));
    group->addChild(std::move(LowShelfEnabled));
    group->addChild(std::move(HighShelfEnabled));
    group->addChild(std::move(CutoffEnabled));
    group->addChild(std::move(LateStageTap));
    group->addChild(std::move(Interpolation));

    params.push_back(std::move(group));
    return {params.begin(), params.end()};
}

void AudioPluginAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    auto it = paramCache.find(parameterID);
    if (it == paramCache.end())
        return;
    auto param = it->second;
    auto normalised_value = param->convertTo0to1(newValue);
    auto param_enum = map.find(parameterID);
    if (param_enum == map.end())
        return;

    if (parameterID == "LateDiffusionEnabled" || parameterID == "LateStageTap")
        requestPresetTransition(20);

    auto dspValue = newValue;

    // Keep the UI/APVTS parameter at the full 0..1 range so the knob still reaches 100%,
    // but cap the actual Early Diffusion allpass feedback sent to DSP at 0.92.
    // This avoids the abrupt tonal/ringing change that happens near feedback = 1.0.
    if (parameterID == "DiffusionFeedback")
        dspValue = juce::jlimit(0.0f, 0.92f, newValue * 0.92f);

    queue.enqueue({normalised_value, dspValue, param_enum->second});
    // DBG(parameterID << ": " << newValue);
}

void AudioPluginAudioProcessor::setPreset(cloudPresets::preset preset)
{
    treeState.getParameter("InputMix")->setValueNotifyingHost(preset.InputMix);
    treeState.getParameter("PreDelay")->setValueNotifyingHost(preset.PreDelay);
    treeState.getParameter("HighPass")->setValueNotifyingHost(preset.HighPass);
    treeState.getParameter("LowPass")->setValueNotifyingHost(preset.LowPass);
    treeState.getParameter("TapEnabled")->setValueNotifyingHost(1.0f);
    treeState.getParameter("TapCount")->setValueNotifyingHost(preset.TapCount);
    treeState.getParameter("TapLength")->setValueNotifyingHost(preset.TapLength);
    treeState.getParameter("TapGain")->setValueNotifyingHost(preset.TapGain);
    treeState.getParameter("TapDecay")->setValueNotifyingHost(preset.TapDecay);
    treeState.getParameter("DiffusionEnabled")->setValueNotifyingHost(preset.DiffusionEnabled);
    treeState.getParameter("DiffusionStages")->setValueNotifyingHost(preset.DiffusionStages);
    treeState.getParameter("DiffusionDelay")->setValueNotifyingHost(preset.DiffusionDelay);
    treeState.getParameter("DiffusionFeedback")->setValueNotifyingHost(preset.DiffusionFeedback);
    treeState.getParameter("LateDelayEnabled")->setValueNotifyingHost(1.0f);
    treeState.getParameter("LineCount")->setValueNotifyingHost(preset.LineCount);
    treeState.getParameter("LineDelay")->setValueNotifyingHost(preset.LineDelay);
    treeState.getParameter("LineDecay")->setValueNotifyingHost(preset.LineDecay);
    treeState.getParameter("LateDiffusionEnabled")
        ->setValueNotifyingHost(preset.LateDiffusionEnabled);
    treeState.getParameter("LateDiffusionStages")
        ->setValueNotifyingHost(preset.LateDiffusionStages);
    treeState.getParameter("LateDiffusionDelay")->setValueNotifyingHost(preset.LateDiffusionDelay);
    treeState.getParameter("LateDiffusionFeedback")
        ->setValueNotifyingHost(preset.LateDiffusionFeedback);
    treeState.getParameter("PostLowShelfGain")->setValueNotifyingHost(preset.PostLowShelfGain);
    treeState.getParameter("PostLowShelfFrequency")
        ->setValueNotifyingHost(preset.PostLowShelfFrequency);
    treeState.getParameter("PostHighShelfGain")->setValueNotifyingHost(preset.PostHighShelfGain);
    treeState.getParameter("PostHighShelfFrequency")
        ->setValueNotifyingHost(preset.PostHighShelfFrequency);
    treeState.getParameter("PostCutoffFrequency")
        ->setValueNotifyingHost(preset.PostCutoffFrequency);
    treeState.getParameter("EarlyDiffusionModAmount")
        ->setValueNotifyingHost(preset.EarlyDiffusionModAmount);
    treeState.getParameter("EarlyDiffusionModRate")
        ->setValueNotifyingHost(preset.EarlyDiffusionModRate);
    treeState.getParameter("LineModAmount")->setValueNotifyingHost(preset.LineModAmount);
    treeState.getParameter("LineModRate")->setValueNotifyingHost(preset.LineModRate);
    treeState.getParameter("LateDiffusionModAmount")
        ->setValueNotifyingHost(preset.LateDiffusionModAmount);
    treeState.getParameter("LateDiffusionModRate")
        ->setValueNotifyingHost(preset.LateDiffusionModRate);
    treeState.getParameter("TapSeed")->setValueNotifyingHost(preset.TapSeed);
    treeState.getParameter("DiffusionSeed")->setValueNotifyingHost(preset.DiffusionSeed);
    treeState.getParameter("DelaySeed")->setValueNotifyingHost(preset.DelaySeed);
    treeState.getParameter("PostDiffusionSeed")->setValueNotifyingHost(preset.PostDiffusionSeed);
    treeState.getParameter("CrossSeed")->setValueNotifyingHost(preset.CrossSeed);
    treeState.getParameter("DryOut")->setValueNotifyingHost(preset.DryOut);
    treeState.getParameter("PredelayOut")->setValueNotifyingHost(preset.PredelayOut);
    treeState.getParameter("EarlyOut")->setValueNotifyingHost(preset.EarlyOut);
    treeState.getParameter("MainOut")->setValueNotifyingHost(preset.MainOut);
    if (auto *widthParam = treeState.getParameter("Width"))
        widthParam->setValueNotifyingHost(widthParam->convertTo0to1(1.0f)); // actual width = 1.0
    treeState.getParameter("HiPassEnabled")->setValueNotifyingHost(preset.HiPassEnabled);
    treeState.getParameter("LowPassEnabled")->setValueNotifyingHost(preset.LowPassEnabled);
    treeState.getParameter("LowShelfEnabled")->setValueNotifyingHost(preset.LowShelfEnabled);
    treeState.getParameter("HighShelfEnabled")->setValueNotifyingHost(preset.HighShelfEnabled);
    treeState.getParameter("CutoffEnabled")->setValueNotifyingHost(preset.CutoffEnabled);
    treeState.getParameter("LateStageTap")->setValueNotifyingHost(preset.LateStageTap);
    treeState.getParameter("Interpolation")->setValueNotifyingHost(preset.Interpolation);
}

void AudioPluginAudioProcessor::setCurrentPresetDisplayName(const juce::String &name)
{
    currentPresetDisplayName = name;
}

juce::String AudioPluginAudioProcessor::getCurrentPresetDisplayName() const
{
    return currentPresetDisplayName;
}

void AudioPluginAudioProcessor::reset()
{
    DBG("Reset CloudReverb plugin");
    reverb.ClearBuffers();
}

void AudioPluginAudioProcessor::requestPresetTransition(int fadeTimeMs)
{
    presetTransitionFadeMs.store(fadeTimeMs, std::memory_order_release);
    presetTransitionRequested.store(true, std::memory_order_release);
}

void AudioPluginAudioProcessor::handleAsyncUpdate()
{
    AudioPluginAudioProcessorEditor *editor =
        (AudioPluginAudioProcessorEditor *)(getActiveEditor());
    if (!editor)
        return;
    editor->restoreUIstate();
}

#include "PluginProcessor.h"

#include "../audio_engine/AudioLib/ValueTables.h"
#include "../audio_engine/FastSin.h"
#include "PluginEditor.h"
juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

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
      treeState(*this, nullptr, juce::Identifier("CloudReverb"), createParameterLayout()),
      reverb(48000)
{
    AudioLib::ValueTables::Init();
    CloudSeed::FastSin::Init();
    reverb.ClearBuffers(); // clear buffers before we start do dsp stuff.

    for (auto param : getParameters())
    {
        auto paramWithID = dynamic_cast<juce::AudioProcessorParameterWithID *>(param);
        treeState.addParameterListener(paramWithID->paramID, this);
    }

    map.insert({"InputMix", Parameter::InputMix});
    map.insert({"PreDelay", Parameter::PreDelay});
    map.insert({"HighPass", Parameter::HighPass});
    map.insert({"LowPass", Parameter::LowPass});
    map.insert({"TapCount", Parameter::TapCount});
    map.insert({"TapLength", Parameter::TapLength});
    map.insert({"TapGain", Parameter::TapGain});
    map.insert({"TapDecay", Parameter::TapDecay});
    map.insert({"DiffusionEnabled", Parameter::DiffusionEnabled});
    map.insert({"DiffusionStages", Parameter::DiffusionStages});
    map.insert({"DiffusionDelay", Parameter::DiffusionDelay});
    map.insert({"DiffusionFeedback", Parameter::DiffusionFeedback});
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
    map.insert({"HiPassEnabled", Parameter::HiPassEnabled});
    map.insert({"LowPassEnabled", Parameter::LowPassEnabled});
    map.insert({"LowShelfEnabled", Parameter::LowShelfEnabled});
    map.insert({"HighShelfEnabled", Parameter::HighShelfEnabled});
    map.insert({"CutoffEnabled", Parameter::CutoffEnabled});
    map.insert({"LateStageTap", Parameter::LateStageTap});
    map.insert({"Interpolation", Parameter::Interpolation});

    // https://github.com/ValdemarOrn/CloudSeed/blob/master/Factory%20Programs/Chorus%20Delay.json
    treeState.getParameter("InputMix")->setValueNotifyingHost(0.0);
    treeState.getParameter("PreDelay")->setValueNotifyingHost(0.070000000298023224);
    treeState.getParameter("HighPass")->setValueNotifyingHost(0.0);
    treeState.getParameter("LowPass")->setValueNotifyingHost(0.29000008106231689);
    treeState.getParameter("TapCount")->setValueNotifyingHost(0.35);
    treeState.getParameter("TapLength")->setValueNotifyingHost(1.0);
    treeState.getParameter("TapGain")->setValueNotifyingHost(1.0);
    treeState.getParameter("TapDecay")->setValueNotifyingHost(0.86500012874603271);
    treeState.getParameter("DiffusionEnabled")->setValueNotifyingHost(1.0);
    treeState.getParameter("DiffusionStages")->setValueNotifyingHost(0.4285714328289032);
    treeState.getParameter("DiffusionDelay")->setValueNotifyingHost(0.43500006198883057);
    treeState.getParameter("DiffusionFeedback")->setValueNotifyingHost(0.725000262260437);
    treeState.getParameter("LineCount")->setValueNotifyingHost(1.0);
    treeState.getParameter("LineDelay")->setValueNotifyingHost(0.68499988317489624);
    treeState.getParameter("LineDecay")->setValueNotifyingHost(0.68000012636184692);
    treeState.getParameter("LateDiffusionEnabled")->setValueNotifyingHost(1.0);
    treeState.getParameter("LateDiffusionStages")->setValueNotifyingHost(0.28571429848670959);
    treeState.getParameter("LateDiffusionDelay")->setValueNotifyingHost(0.54499995708465576);
    treeState.getParameter("LateDiffusionFeedback")->setValueNotifyingHost(0.65999996662139893);
    treeState.getParameter("PostLowShelfGain")->setValueNotifyingHost(0.5199999213218689);
    treeState.getParameter("PostLowShelfFrequency")->setValueNotifyingHost(0.31499990820884705);
    treeState.getParameter("PostHighShelfGain")->setValueNotifyingHost(0.83500003814697266);
    treeState.getParameter("PostHighShelfFrequency")->setValueNotifyingHost(0.73000013828277588);
    treeState.getParameter("PostCutoffFrequency")->setValueNotifyingHost(0.73499983549118042);
    treeState.getParameter("EarlyDiffusionModAmount")->setValueNotifyingHost(0.50000005960464478);
    treeState.getParameter("EarlyDiffusionModRate")->setValueNotifyingHost(0.42500010132789612);
    treeState.getParameter("LineModAmount")->setValueNotifyingHost(0.59000003337860107);
    treeState.getParameter("LineModRate")->setValueNotifyingHost(0.46999993920326233);
    treeState.getParameter("LateDiffusionModAmount")->setValueNotifyingHost(0.619999885559082);
    treeState.getParameter("LateDiffusionModRate")->setValueNotifyingHost(0.42500019073486328);
    treeState.getParameter("TapSeed")->setValueNotifyingHost(0.001149);
    treeState.getParameter("DiffusionSeed")->setValueNotifyingHost(0.000188);
    treeState.getParameter("DelaySeed")->setValueNotifyingHost(0.000336);
    treeState.getParameter("PostDiffusionSeed")->setValueNotifyingHost(0.000499);
    treeState.getParameter("CrossSeed")->setValueNotifyingHost(0.0);
    treeState.getParameter("DryOut")->setValueNotifyingHost(0.94499987363815308);
    treeState.getParameter("PredelayOut")->setValueNotifyingHost(0.0);
    treeState.getParameter("EarlyOut")->setValueNotifyingHost(0.77999997138977051);
    treeState.getParameter("MainOut")->setValueNotifyingHost(0.74500006437301636);
    treeState.getParameter("HiPassEnabled")->setValueNotifyingHost(0.0);
    treeState.getParameter("LowPassEnabled")->setValueNotifyingHost(0.0);
    treeState.getParameter("LowShelfEnabled")->setValueNotifyingHost(0.0);
    treeState.getParameter("HighShelfEnabled")->setValueNotifyingHost(0.0);
    treeState.getParameter("CutoffEnabled")->setValueNotifyingHost(1.0);
    treeState.getParameter("LateStageTap")->setValueNotifyingHost(1.0);
    treeState.getParameter("Interpolation")->setValueNotifyingHost(0.);
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

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // we only process stero signal, so we clear unused output buffers.
    for (auto i = 2; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    Message message;
    while (queue.try_dequeue(message))
    {
        reverb.updateParameter(message.param, message.newScaledValue, message.newNormalisedValue);
    }
    // real dsp
    const float *const *in_sig = buffer.getArrayOfReadPointers();
    float *const *out_sig = buffer.getArrayOfWritePointers();
    reverb.Process(in_sig, out_sig, buffer.getNumSamples());
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
    auto state = treeState.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void AudioPluginAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    std::cout << xmlState->toString();

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(treeState.state.getType()))
            treeState.replaceState(juce::ValueTree::fromXml(*xmlState));
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
                          : juce::String(10 * log(value), numberOfDecimalPlaces) + " " + "dB";     \
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

    auto TapCount = MAKE_PARAMETER_INT(TapCount, 1, 50, 1, taps);

    auto TapLength = MAKE_PARAMETER_INT(TapLength, 0, 500, 0, ms);

    auto TapGain = MAKE_PARAMETER_FLOAT(TapGain, 0.0f, 1.0f, 0.5f, 100, );

    auto TapDecay = MAKE_PARAMETER_LINEAR_FLOAT(TapDecay, 0.f, 1.f, 0.f, );

    auto DiffusionEnabled =
        std::make_unique<juce::AudioParameterBool>("DiffusionEnabled", "DiffusionEnabled", false);

    auto DiffusionStages = MAKE_PARAMETER_INT(DiffusionStages, 1, 8, 1, stages);

    auto DiffusionDelay = MAKE_PARAMETER_INT(DiffusionDelay, 10, 100, 10, ms);

    auto DiffusionFeedback = MAKE_PARAMETER_LINEAR_FLOAT(DiffusionFeedback, 0.f, 1.f, 0.f, );

    auto LineCount = MAKE_PARAMETER_INT(LineCount, 1, 12, 1, parallel delay);

    auto LineDelay = MAKE_PARAMETER_FLOAT(LineDelay, 20.0f, 1000.0f, 20.f, 100, ms);

    auto LineDecay = MAKE_PARAMETER_FLOAT(LineDecay, 0.05f, 60.f, 1.f, 1000, ms);

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

    auto LineModAmount = MAKE_PARAMETER_LINEAR_FLOAT(LineModAmount, 0.f, 2.5f, 0.f, );

    auto LineModRate = MAKE_PARAMETER_FLOAT(LineModRate, 0.f, 5.f, 0.5f, 100, Hz);

    auto LateDiffusionModAmount =
        MAKE_PARAMETER_LINEAR_FLOAT(LateDiffusionModAmount, 0.f, 2.5f, 0.f, );

    auto LateDiffusionModRate = MAKE_PARAMETER_FLOAT(LateDiffusionModRate, 0.f, 5.f, 0.5f, 100, Hz);

    auto TapSeed = MAKE_PARAMETER_INT(TapSeed, 1, 1000000, 1, );

    auto DiffusionSeed = MAKE_PARAMETER_INT(DiffusionSeed, 1, 1000000, 1, );

    auto DelaySeed = MAKE_PARAMETER_INT(DelaySeed, 1, 1000000, 1, );

    auto PostDiffusionSeed = MAKE_PARAMETER_INT(PostDiffusionSeed, 1, 1000000, 1, );

    auto CrossSeed = MAKE_PARAMETER_LINEAR_FLOAT(CrossSeed, 0.f, 1.f, 0.f, );

    auto DryOut = MAKE_PARAMETER_DB_FLOAT(DryOut, 0.f, 1.f, 1.f, 100);

    auto PredelayOut = MAKE_PARAMETER_DB_FLOAT(PredelayOut, 0.f, 1.f, 0.f, 100);

    auto EarlyOut = MAKE_PARAMETER_DB_FLOAT(EarlyOut, 0.f, 1.f, 0.f, 100);

    auto MainOut = MAKE_PARAMETER_DB_FLOAT(MainOut, 0.f, 1.f, 0.f, 100);

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
    group->addChild(std::move(TapCount));
    group->addChild(std::move(TapLength));
    group->addChild(std::move(TapGain));
    group->addChild(std::move(TapDecay));
    group->addChild(std::move(DiffusionEnabled));
    group->addChild(std::move(DiffusionStages));
    group->addChild(std::move(DiffusionDelay));
    group->addChild(std::move(DiffusionFeedback));
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
    auto param = treeState.getParameter(parameterID);
    auto normalised_value = param->convertTo0to1(newValue);
    auto param_enum = map.find(parameterID);
    if (param_enum == map.end())
        return;
    queue.enqueue({normalised_value, newValue, param_enum->second});
    DBG(parameterID << ": " << newValue);
}

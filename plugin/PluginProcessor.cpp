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
      reverb(48000) {
    AudioLib::ValueTables::Init();
    CloudSeed::FastSin::Init();
    reverb.ClearBuffers();  // clear buffers before we start do dsp stuff.

    for (auto param : getParameters()) {
        auto paramWithID = dynamic_cast<juce::AudioProcessorParameterWithID*>(param);
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

AudioPluginAudioProcessor::~AudioPluginAudioProcessor() {
}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const {
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool AudioPluginAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const {
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms() {
    return 1;  // NB: some hosts don't cope very well if you tell them there are 0 programs,
               // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram() {
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram(int index) {
    juce::ignoreUnused(index);
}

const juce::String AudioPluginAudioProcessor::getProgramName(int index) {
    juce::ignoreUnused(index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName(int index, const juce::String& newName) {
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused(samplesPerBlock);
    reverb.SetSamplerate(sampleRate);
}

void AudioPluginAudioProcessor::releaseResources() {
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}

void AudioPluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                             juce::MidiBuffer& midiMessages) {
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // we only process stero signal, so we clear unused output buffers.
    for (auto i = 2; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    Message message;
    while (queue.try_dequeue(message)) {
        reverb.updateParameter(message.param, message.newScaledValue, message.newNormalisedValue);
    }
    // real dsp
    const float** in_sig = buffer.getArrayOfReadPointers();
    float** out_sig = buffer.getArrayOfWritePointers();
    reverb.Process(in_sig, out_sig, buffer.getNumSamples());
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const {
    return true;  // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor() {
    return new AudioPluginAudioProcessorEditor(*this);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = treeState.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void AudioPluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
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
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new AudioPluginAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    std::vector<std::unique_ptr<juce::AudioProcessorParameterGroup>> params;

    auto InputMix = std::make_unique<juce::AudioParameterFloat>(
        "InputMix", "InputMix", juce::NormalisableRange<float>(0.0f, 1.0f),
        0.5f);
    auto PreDelay = std::make_unique<juce::AudioParameterInt>(
        "PreDelay", "PreDelay", 0, 1000, 0);  // ms
    auto HighPass = std::make_unique<juce::AudioParameterFloat>(
        "HighPass", "HighPass",
        juce::NormalisableRange<float>(
            20.0f, 1000.0f,
            [](float start, float end, float value) {
                return start + (std::pow(16, value) - 1) / 15 * (end - start);
            },
            [](float start, float end, float value) {
                return std::log((value - start) / (end - start) * 15 + 1) /
                       std::log(16);
            }),
        20.0f);
    auto LowPass = std::make_unique<juce::AudioParameterFloat>(
        "LowPass", "LowPass",
        juce::NormalisableRange<float>(
            400.0f, 20000.0f,
            [](float start, float end, float value) {
                return start + (std::pow(16, value) - 1) / 15 * (end - start);
            },
            [](float start, float end, float value) {
                return std::log((value - start) / (end - start) * 15 + 1) /
                       std::log(16);
            }),
        20000.0f);
    auto TapCount = std::make_unique<juce::AudioParameterInt>(
        "TapCount", "TapCount", 1, 50, 1);
    auto TapLength = std::make_unique<juce::AudioParameterInt>(
        "TapLength", "TapLength", 0, 500, 1);
    auto TapGain = std::make_unique<juce::AudioParameterFloat>(
        "TapGain", "TapGain",
        juce::NormalisableRange<float>(
            0.0f, 1.0f,
            [](float start, float end, float value) {
                return start + (std::pow(100, value) - 1) / 99 * (end - start);
            },
            [](float start, float end, float value) {
                return std::log((value - start) / (end - start) * 99 + 1) /
                       std::log(100);
            }),
        0.5f);
    auto TapDecay = std::make_unique<juce::AudioParameterFloat>(
        "TapDecay", "TapDecay", juce::NormalisableRange<float>(0.0f, 1.0f),
        0.5f);
    auto DiffusionEnabled = std::make_unique<juce::AudioParameterBool>(
        "DiffusionEnabled", "DiffusionEnabled", false);
    auto DiffusionStages = std::make_unique<juce::AudioParameterInt>(
        "DiffusionStages", "DiffusionStages", 1, 8, 1);
    auto DiffusionDelay = std::make_unique<juce::AudioParameterInt>(
        "DiffusionDelay", "DiffusionDelay", 10, 100, 10);  // ms
    auto DiffusionFeedback = std::make_unique<juce::AudioParameterFloat>(
        "DiffusionFeedback", "DiffusionFeedback",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f);
    auto LineCount = std::make_unique<juce::AudioParameterInt>(
        "LineCount", "LineCount", 1, 12, 1);
    auto LineDelay = std::make_unique<juce::AudioParameterFloat>(
        "LineDelay", "LineDelay",
        juce::NormalisableRange<float>(
            20.0f, 1000.0f,
            [](float start, float end, float value) {
                return start + (std::pow(100, value) - 1) / 99 * (end - start);
            },
            [](float start, float end, float value) {
                return std::log((value - start) / (end - start) * 99 + 1) /
                       std::log(100);
            }),
        20.0f);
    auto LineDecay = std::make_unique<juce::AudioParameterFloat>(
        "LineDecay", "LineDecay",
        juce::NormalisableRange<float>(
            0.05f, 60.0f,
            [](float start, float end, float value) {
                return start +
                       (std::pow(1000, value) - 1) / 999 * (end - start);
            },
            [](float start, float end, float value) {
                return std::log((value - start) / (end - start) * 999 + 1) /
                       std::log(1000);
            }),
        1.0f);
    auto LateDiffusionEnabled = std::make_unique<juce::AudioParameterBool>(
        "LateDiffusionEnabled", "LateDiffusionEnabled", false);
    auto LateDiffusionStages = std::make_unique<juce::AudioParameterInt>(
        "LateDiffusionStages", "LateDiffusionStages", 1, 8, 1);
    auto LateDiffusionDelay = std::make_unique<juce::AudioParameterInt>(
        "LateDiffusionDelay", "LateDiffusionDelay", 10, 100, 10);
    auto LateDiffusionFeedback = std::make_unique<juce::AudioParameterFloat>(
        "LateDiffusionFeedback", "LateDiffusionFeedback",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f);
    auto PostLowShelfGain = std::make_unique<juce::AudioParameterFloat>(
        "PostLowShelfGain", "PostLowShelfGain",
        juce::NormalisableRange<float>(
            0.0f, 1.0f,
            [](float start, float end, float value) {
                return start + (std::pow(100, value) - 1) / 99 * (end - start);
            },
            [](float start, float end, float value) {
                return std::log((value - start) / (end - start) * 99 + 1) /
                       std::log(100);
            }),
        0.5f);
    auto PostLowShelfFrequency = std::make_unique<juce::AudioParameterFloat>(
        "PostLowShelfFrequency", "PostLowShelfFrequency",
        juce::NormalisableRange<float>(
            20.0f, 1000.0f,
            [](float start, float end, float value) {
                return start + (std::pow(16, value) - 1) / 15 * (end - start);
            },
            [](float start, float end, float value) {
                return std::log((value - start) / (end - start) * 15 + 1) /
                       std::log(16);
            }),
        1000.0f);
    auto PostHighShelfGain = std::make_unique<juce::AudioParameterFloat>(
        "PostHighShelfGain", "PostHighShelfGain",
        juce::NormalisableRange<float>(
            0.0f, 1.0f,
            [](float start, float end, float value) {
                return start + (std::pow(100, value) - 1) / 99 * (end - start);
            },
            [](float start, float end, float value) {
                return std::log((value - start) / (end - start) * 99 + 1) /
                       std::log(100);
            }),
        0.5f);
    auto PostHighShelfFrequency = std::make_unique<juce::AudioParameterFloat>(
        "PostHighShelfFrequency", "PostHighShelfFrequency",
        juce::NormalisableRange<float>(
            400.0f, 20000.0f,
            [](float start, float end, float value) {
                return start + (std::pow(16, value) - 1) / 15 * (end - start);
            },
            [](float start, float end, float value) {
                return std::log((value - start) / (end - start) * 15 + 1) /
                       std::log(16);
            }),
        20000.0f);
    auto PostCutoffFrequency = std::make_unique<juce::AudioParameterFloat>(
        "PostCutoffFrequency", "PostCutoffFrequency",
        juce::NormalisableRange<float>(
            400.0f, 20000.0f,
            [](float start, float end, float value) {
                return start + (std::pow(16, value) - 1) / 15 * (end - start);
            },
            [](float start, float end, float value) {
                return std::log((value - start) / (end - start) * 15 + 1) /
                       std::log(16);
            }),
        20000.0f);
    auto EarlyDiffusionModAmount = std::make_unique<juce::AudioParameterFloat>(
        "EarlyDiffusionModAmount", "EarlyDiffusionModAmount",
        juce::NormalisableRange<float>(0.0f, 2.5f), 0.5f);
    auto EarlyDiffusionModRate = std::make_unique<juce::AudioParameterFloat>(
        "EarlyDiffusionModRate", "EarlyDiffusionModRate",
        juce::NormalisableRange<float>(
            0.0f, 5.0f,
            [](float start, float end, float value) {
                return start + (std::pow(100, value) - 1) / 99 * (end - start);
            },
            [](float start, float end, float value) {
                return std::log((value - start) / (end - start) * 99 + 1) /
                       std::log(100);
            }),
        0.5f);
    auto LineModAmount = std::make_unique<juce::AudioParameterFloat>(
        "LineModAmount", "LineModAmount",
        juce::NormalisableRange<float>(0.0f, 2.5f), 0.5f);
    auto LineModRate = std::make_unique<juce::AudioParameterFloat>(
        "LineModRate", "LineModRate",
        juce::NormalisableRange<float>(
            0.0f, 5.0f,
            [](float start, float end, float value) {
                return start + (std::pow(100, value) - 1) / 99 * (end - start);
            },
            [](float start, float end, float value) {
                return std::log((value - start) / (end - start) * 99 + 1) /
                       std::log(100);
            }),
        0.5f);
    auto LateDiffusionModAmount = std::make_unique<juce::AudioParameterFloat>(
        "LateDiffusionModAmount", "LateDiffusionModAmount",
        juce::NormalisableRange<float>(0.0f, 2.5f), 0.5f);
    auto LateDiffusionModRate = std::make_unique<juce::AudioParameterFloat>(
        "LateDiffusionModRate", "LateDiffusionModRate",
        juce::NormalisableRange<float>(
            0.0f, 5.0f,
            [](float start, float end, float value) {
                return start + (std::pow(100, value) - 1) / 99 * (end - start);
            },
            [](float start, float end, float value) {
                return std::log((value - start) / (end - start) * 99 + 1) /
                       std::log(100);
            }),
        0.5f);
    auto TapSeed = std::make_unique<juce::AudioParameterInt>(
        "TapSeed", "TapSeed", 1, 1000000, 1);
    auto DiffusionSeed = std::make_unique<juce::AudioParameterInt>(
        "DiffusionSeed", "DiffusionSeed", 1, 1000000, 1);
    auto DelaySeed = std::make_unique<juce::AudioParameterInt>(
        "DelaySeed", "DelaySeed", 1, 1000000, 1);
    auto PostDiffusionSeed = std::make_unique<juce::AudioParameterInt>(
        "PostDiffusionSeed", "PostDiffusionSeed", 1, 1000000, 1);
    auto CrossSeed = std::make_unique<juce::AudioParameterFloat>(
        "CrossSeed", "CrossSeed", juce::NormalisableRange<float>(0.0f, 1.0f),
        0.5f);
    auto DryOut = std::make_unique<juce::AudioParameterFloat>(
        "DryOut", "DryOut",
        juce::NormalisableRange<float>(
            0.0f, 1.0f,
            [](float start, float end, float value) {
                return start + (std::pow(100, value) - 1) / 99 * (end - start);
            },
            [](float start, float end, float value) {
                return std::log((value - start) / (end - start) * 99 + 1) /
                       std::log(100);
            }),
        0.5f);
    auto PredelayOut = std::make_unique<juce::AudioParameterFloat>(
        "PredelayOut", "PredelayOut",
        juce::NormalisableRange<float>(
            0.0f, 1.0f,
            [](float start, float end, float value) {
                return start + (std::pow(100, value) - 1) / 99 * (end - start);
            },
            [](float start, float end, float value) {
                return std::log((value - start) / (end - start) * 99 + 1) /
                       std::log(100);
            }),
        0.5f);
    auto EarlyOut = std::make_unique<juce::AudioParameterFloat>(
        "EarlyOut", "EarlyOut",
        juce::NormalisableRange<float>(
            0.0f, 1.0f,
            [](float start, float end, float value) {
                return start + (std::pow(100, value) - 1) / 99 * (end - start);
            },
            [](float start, float end, float value) {
                return std::log((value - start) / (end - start) * 99 + 1) /
                       std::log(100);
            }),
        0.5f);
    auto MainOut = std::make_unique<juce::AudioParameterFloat>(
        "MainOut", "MainOut",
        juce::NormalisableRange<float>(
            0.0f, 1.0f,
            [](float start, float end, float value) {
                return start + (std::pow(100, value) - 1) / 99 * (end - start);
            },
            [](float start, float end, float value) {
                return std::log((value - start) / (end - start) * 99 + 1) /
                       std::log(100);
            }),
        0.5f);
    auto HiPassEnabled = std::make_unique<juce::AudioParameterBool>(
        "HiPassEnabled", "HiPassEnabled", false);
    auto LowPassEnabled = std::make_unique<juce::AudioParameterBool>(
        "LowPassEnabled", "LowPassEnabled", false);
    auto LowShelfEnabled = std::make_unique<juce::AudioParameterBool>(
        "LowShelfEnabled", "LowShelfEnabled", false);
    auto HighShelfEnabled = std::make_unique<juce::AudioParameterBool>(
        "HighShelfEnabled", "HighShelfEnabled", false);
    auto CutoffEnabled = std::make_unique<juce::AudioParameterBool>(
        "CutoffEnabled", "CutoffEnabled", false);
    auto LateStageTap = std::make_unique<juce::AudioParameterFloat>(
        "LateStageTap", "LateStageTap",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f);
    auto Interpolation = std::make_unique<juce::AudioParameterBool>(
        "Interpolation", "Interpolation", false);
    auto group = std::make_unique<juce::AudioProcessorParameterGroup>(
        "CloudReverb_ID", "CloudReverb", "|");

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

void AudioPluginAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue) {
    auto param = treeState.getParameter(parameterID);
    auto normalised_value = param->convertTo0to1(newValue);
    auto param_enum = map.find(parameterID);
    if (param_enum == map.end())
        return;
    queue.enqueue({normalised_value, newValue, param_enum->second});
    std::cout << parameterID << ": " << newValue << std::endl;
}

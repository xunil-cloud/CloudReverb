#include "PluginProcessor.h"

#include "../audio_engine/AudioLib/ValueTables.h"
#include "../audio_engine/FastSin.h"
#include "PluginEditor.h"

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
      reverb(44100),
      treeState(*this, nullptr, juce::Identifier("CloudReverb"),
                {
                    std::make_unique<juce::AudioParameterFloat>(
                        "gain",  // parameterID
                        "Gain",  // parameter name
                        0.0f,    // minimum value
                        1.0f,    // maximum value
                        0.5f)    // default value
                }) {
    AudioLib::ValueTables::Init();
    CloudSeed::FastSin::Init();
    reverb.ClearBuffers();  // clear buffers before we start do dsp stuff.

    // https://github.com/ValdemarOrn/CloudSeed/blob/master/Factory%20Programs/Hyperplane.json

    reverb.SetParameter(Parameter::InputMix, 0.1549999862909317);
    reverb.SetParameter(Parameter::PreDelay, 0.0);
    reverb.SetParameter(Parameter::HighPass, 0.57999998331069946);
    reverb.SetParameter(Parameter::LowPass, 0.9100000262260437);
    reverb.SetParameter(Parameter::TapCount, 0.41499990224838257);
    reverb.SetParameter(Parameter::TapLength, 0.43999996781349182);
    reverb.SetParameter(Parameter::TapGain, 1.0);
    reverb.SetParameter(Parameter::TapDecay, 1.0);
    reverb.SetParameter(Parameter::DiffusionEnabled, 1.0);
    reverb.SetParameter(Parameter::DiffusionStages, 0.4285714328289032);
    reverb.SetParameter(Parameter::DiffusionDelay, 0.27500024437904358);
    reverb.SetParameter(Parameter::DiffusionFeedback, 0.660000205039978);
    reverb.SetParameter(Parameter::LineCount, 0.72727274894714355);
    reverb.SetParameter(Parameter::LineDelay, 0.22500017285346985);
    reverb.SetParameter(Parameter::LineDecay, 0.794999897480011);
    reverb.SetParameter(Parameter::LateDiffusionEnabled, 1.0);
    reverb.SetParameter(Parameter::LateDiffusionStages, 1.0);
    reverb.SetParameter(Parameter::LateDiffusionDelay, 0.22999951243400574);
    reverb.SetParameter(Parameter::LateDiffusionFeedback, 0.59499990940093994);
    reverb.SetParameter(Parameter::PostLowShelfGain, 0.95999979972839355);
    reverb.SetParameter(Parameter::PostLowShelfFrequency, 0.23999994993209839);
    reverb.SetParameter(Parameter::PostHighShelfGain, 0.97500002384185791);
    reverb.SetParameter(Parameter::PostHighShelfFrequency, 0.78499996662139893);
    reverb.SetParameter(Parameter::PostCutoffFrequency, 0.87999981641769409);
    reverb.SetParameter(Parameter::EarlyDiffusionModAmount, 0.13499999046325684);
    reverb.SetParameter(Parameter::EarlyDiffusionModRate, 0.29000008106231689);
    reverb.SetParameter(Parameter::LineModAmount, 0.53999996185302734);
    reverb.SetParameter(Parameter::LineModRate, 0.44999989867210388);
    reverb.SetParameter(Parameter::LateDiffusionModAmount, 0.15999998152256012);
    reverb.SetParameter(Parameter::LateDiffusionModRate, 0.56000012159347534);
    reverb.SetParameter(Parameter::TapSeed, 0.00048499999684281647);
    reverb.SetParameter(Parameter::DiffusionSeed, 0.00020799999765586108);
    reverb.SetParameter(Parameter::DelaySeed, 0.00034699999378062785);
    reverb.SetParameter(Parameter::PostDiffusionSeed, 0.00037200000951997936);
    reverb.SetParameter(Parameter::CrossSeed, 0.800000011920929);
    reverb.SetParameter(Parameter::DryOut, 0.86500018835067749);
    reverb.SetParameter(Parameter::PredelayOut, 0.0);
    reverb.SetParameter(Parameter::EarlyOut, 0.8200000524520874);
    reverb.SetParameter(Parameter::MainOut, 0.79500007629394531);
    reverb.SetParameter(Parameter::HiPassEnabled, 1.0);
    reverb.SetParameter(Parameter::LowPassEnabled, 1.0);
    reverb.SetParameter(Parameter::LowShelfEnabled, 1.0);
    reverb.SetParameter(Parameter::HighShelfEnabled, 1.0);
    reverb.SetParameter(Parameter::CutoffEnabled, 1.0);
    reverb.SetParameter(Parameter::LateStageTap, 1.0);
    reverb.SetParameter(Parameter::Interpolation, 0.0);

    addParameter(InputMix = new juce::AudioParameterFloat(
                     "InputMix", "InputMix",
                     juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f));
    addParameter(PreDelay = new juce::AudioParameterInt("PreDelay", "PreDelay", 0,
                                                        1000, 0));  // ms
    addParameter(
        HighPass = new juce::AudioParameterFloat(
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
            20.0f));
    addParameter(
        LowPass = new juce::AudioParameterFloat(
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
            20000.0f));
    addParameter(
        TapCount = new juce::AudioParameterInt("TapCount", "TapCount", 1, 50, 1));
    addParameter(TapLength = new juce::AudioParameterInt("TapLength", "TapLength",
                                                         0, 500, 1));
    addParameter(
        TapGain = new juce::AudioParameterFloat(
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
            0.5f));
    addParameter(TapDecay = new juce::AudioParameterFloat(
                     "TapDecay", "TapDecay",
                     juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f));
    addParameter(DiffusionEnabled = new juce::AudioParameterBool(
                     "DiffusionEnabled", "DiffusionEnabled", false));
    addParameter(DiffusionStages = new juce::AudioParameterInt(
                     "DiffusionStages", "DiffusionStages", 1, 8, 1));
    addParameter(DiffusionDelay = new juce::AudioParameterInt(
                     "DiffusionDelay", "DiffusionDelay", 10, 100, 10));  // ms
    addParameter(DiffusionFeedback = new juce::AudioParameterFloat(
                     "DiffusionFeedback", "DiffusionFeedback",
                     juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f));
    addParameter(LineCount = new juce::AudioParameterInt("LineCount", "LineCount",
                                                         1, 12, 1));
    addParameter(
        LineDelay = new juce::AudioParameterFloat(
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
            20.0f));
    addParameter(LineDecay = new juce::AudioParameterFloat(
                     "LineDecay", "LineDecay",
                     juce::NormalisableRange<float>(
                         0.05f, 60.0f,
                         [](float start, float end, float value) {
                             return start + (std::pow(1000, value) - 1) / 999 *
                                                (end - start);
                         },
                         [](float start, float end, float value) {
                             return std::log((value - start) / (end - start) * 999 +
                                             1) /
                                    std::log(1000);
                         }),
                     1.0f));
    addParameter(LateDiffusionEnabled = new juce::AudioParameterBool(
                     "LateDiffusionEnabled", "LateDiffusionEnabled", false));
    addParameter(LateDiffusionStages = new juce::AudioParameterInt(
                     "LateDiffusionStages", "LateDiffusionStages", 1, 8, 1));
    addParameter(LateDiffusionDelay = new juce::AudioParameterInt(
                     "LateDiffusionDelay", "LateDiffusionDelay", 10, 100, 10));
    addParameter(LateDiffusionFeedback = new juce::AudioParameterFloat(
                     "LateDiffusionFeedback", "LateDiffusionFeedback",
                     juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f));
    addParameter(
        PostLowShelfGain = new juce::AudioParameterFloat(
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
            0.5f));
    addParameter(
        PostLowShelfFrequency = new juce::AudioParameterFloat(
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
            1000.0f));
    addParameter(
        PostHighShelfGain = new juce::AudioParameterFloat(
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
            0.5f));
    addParameter(
        PostHighShelfFrequency = new juce::AudioParameterFloat(
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
            20000.0f));
    addParameter(
        PostCutoffFrequency = new juce::AudioParameterFloat(
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
            20000.0f));
    addParameter(EarlyDiffusionModAmount = new juce::AudioParameterFloat(
                     "EarlyDiffusionModAmount", "EarlyDiffusionModAmount",
                     juce::NormalisableRange<float>(0.0f, 2.5f), 0.5f));
    addParameter(
        EarlyDiffusionModRate = new juce::AudioParameterFloat(
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
            0.5f));
    addParameter(LineModAmount = new juce::AudioParameterFloat(
                     "LineModAmount", "LineModAmount",
                     juce::NormalisableRange<float>(0.0f, 2.5f), 0.5f));
    addParameter(
        LineModRate = new juce::AudioParameterFloat(
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
            0.5f));
    addParameter(LateDiffusionModAmount = new juce::AudioParameterFloat(
                     "LateDiffusionModAmount", "LateDiffusionModAmount",
                     juce::NormalisableRange<float>(0.0f, 2.5f), 0.5f));
    addParameter(
        LateDiffusionModRate = new juce::AudioParameterFloat(
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
            0.5f));
    addParameter(TapSeed = new juce::AudioParameterInt("TapSeed", "TapSeed", 1,
                                                       1000000, 1));
    addParameter(DiffusionSeed = new juce::AudioParameterInt(
                     "DiffusionSeed", "DiffusionSeed", 1, 1000000, 1));
    addParameter(DelaySeed = new juce::AudioParameterInt("DelaySeed", "DelaySeed",
                                                         1, 1000000, 1));
    addParameter(PostDiffusionSeed = new juce::AudioParameterInt(
                     "PostDiffusionSeed", "PostDiffusionSeed", 1, 1000000, 1));
    addParameter(CrossSeed = new juce::AudioParameterFloat(
                     "CrossSeed", "CrossSeed",
                     juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f));
    addParameter(DryOut = new juce::AudioParameterFloat(
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
                     0.5f));
    addParameter(PredelayOut = new juce::AudioParameterFloat(
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
                     0.5f));
    addParameter(EarlyOut = new juce::AudioParameterFloat(
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
                     0.5f));
    addParameter(MainOut = new juce::AudioParameterFloat(
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
                     0.5f));
    addParameter(HiPassEnabled = new juce::AudioParameterBool(
                     "HiPassEnabled", "HiPassEnabled", false));
    addParameter(LowPassEnabled = new juce::AudioParameterBool(
                     "LowPassEnabled", "LowPassEnabled", false));
    addParameter(LowShelfEnabled = new juce::AudioParameterBool(
                     "LowShelfEnabled", "LowShelfEnabled", false));
    addParameter(HighShelfEnabled = new juce::AudioParameterBool(
                     "HighShelfEnabled", "HighShelfEnabled", false));
    addParameter(CutoffEnabled = new juce::AudioParameterBool(
                     "CutoffEnabled", "CutoffEnabled", false));
    addParameter(LateStageTap = new juce::AudioParameterFloat(
                     "LateStageTap", "LateStageTap",
                     juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f));
    addParameter(Interpolation = new juce::AudioParameterBool(
                     "Interpolation", "Interpolation", false));
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
    juce::ignoreUnused(sampleRate, samplesPerBlock);
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
    juce::ignoreUnused(destData);
}

void AudioPluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused(data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new AudioPluginAudioProcessor();
}

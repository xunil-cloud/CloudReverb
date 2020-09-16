#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "../audio_engine/FastSin.h"
#include "../audio_engine/AudioLib/ValueTables.h"

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), reverb(44100)
{
    AudioLib::ValueTables::Init();
	CloudSeed::FastSin::Init();
    reverb.ClearBuffers(); // clear buffers before we start do dsp stuff.
  
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
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

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

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String AudioPluginAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void AudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void AudioPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // we only process stero signal, so we clear unused output buffers.
    for (auto i = 2; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // real dsp
    const float** in_sig =  buffer.getArrayOfReadPointers();
    float** out_sig =  buffer.getArrayOfWritePointers();
    reverb.Process(in_sig,out_sig, buffer.getNumSamples());
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor (*this);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void AudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}

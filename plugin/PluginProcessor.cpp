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
  /*  
  reverb.SetParameter(Parameter::InputMix, 0.0);
  reverb.SetParameter(Parameter::PreDelay, 0.0);
  reverb.SetParameter(Parameter::HighPass, 0.0);
  reverb.SetParameter(Parameter::LowPass, 0.755000114440918);
  reverb.SetParameter(Parameter::TapCount, 0.41499990224838257);
  reverb.SetParameter(Parameter::TapLength, 0.43999996781349182);
  reverb.SetParameter(Parameter::TapGain, 0.87999999523162842);
  reverb.SetParameter(Parameter::TapDecay, 1.0);
  reverb.SetParameter(Parameter::DiffusionEnabled, 1.0);
  reverb.SetParameter(Parameter::DiffusionStages, 0.71428573131561279);
  reverb.SetParameter(Parameter::DiffusionDelay, 0.335000216960907);
  reverb.SetParameter(Parameter::DiffusionFeedback, 0.660000205039978);
  reverb.SetParameter(Parameter::LineCount, 0.18181818723678589);
  reverb.SetParameter(Parameter::LineDelay, 0.51000016927719116);
  reverb.SetParameter(Parameter::LineDecay, 0.29999998211860657);
  reverb.SetParameter(Parameter::LateDiffusionEnabled, 1.0);
  reverb.SetParameter(Parameter::LateDiffusionStages, 0.4285714328289032);
  reverb.SetParameter(Parameter::LateDiffusionDelay, 0.22999951243400574);
  reverb.SetParameter(Parameter::LateDiffusionFeedback, 0.59499990940093994);
  reverb.SetParameter(Parameter::PostLowShelfGain, 0.87999987602233887);
  reverb.SetParameter(Parameter::PostLowShelfFrequency, 0.19499993324279785);
  reverb.SetParameter(Parameter::PostHighShelfGain, 0.875);
  reverb.SetParameter(Parameter::PostHighShelfFrequency, 0.59000009298324585);
  reverb.SetParameter(Parameter::PostCutoffFrequency, 0.79999983310699463);
  reverb.SetParameter(Parameter::EarlyDiffusionModAmount, 0.13499999046325684);
  reverb.SetParameter(Parameter::EarlyDiffusionModRate, 0.29000008106231689);
  reverb.SetParameter(Parameter::LineModAmount, 0.18999995291233063);
  reverb.SetParameter(Parameter::LineModRate, 0.22999987006187439);
  reverb.SetParameter(Parameter::LateDiffusionModAmount, 0.1249999925494194);
  reverb.SetParameter(Parameter::LateDiffusionModRate, 0.28500008583068848);
  reverb.SetParameter(Parameter::TapSeed, 0.00048499999684281647);
  reverb.SetParameter(Parameter::DiffusionSeed, 0.00020799999765586108);
  reverb.SetParameter(Parameter::DelaySeed, 0.00033499998971819878);
  reverb.SetParameter(Parameter::PostDiffusionSeed, 0.00037200000951997936);
  reverb.SetParameter(Parameter::CrossSeed, 0.42500001192092896);
  reverb.SetParameter(Parameter::DryOut, 1.0);
  reverb.SetParameter(Parameter::PredelayOut, 0.0);
  reverb.SetParameter(Parameter::EarlyOut, 0.8599998950958252);
  reverb.SetParameter(Parameter::MainOut, 0.90500003099441528);
  reverb.SetParameter(Parameter::HiPassEnabled, 0.0);
  reverb.SetParameter(Parameter::LowPassEnabled, 1.0);
  reverb.SetParameter(Parameter::LowShelfEnabled, 0.0);
  reverb.SetParameter(Parameter::HighShelfEnabled, 0.0);
  reverb.SetParameter(Parameter::CutoffEnabled, 0.0);
  reverb.SetParameter(Parameter::LateStageTap, 1.0);
  reverb.SetParameter(Parameter::Interpolation, 1.0);
  */

  reverb.SetParameter(Parameter::InputMix, 0.0);
  reverb.SetParameter(Parameter::PreDelay, 0.0);
  reverb.SetParameter(Parameter::HighPass, 0.0);
  reverb.SetParameter(Parameter::LowPass, 0.74000012874603271);
  reverb.SetParameter(Parameter::TapCount, 1.0);
  reverb.SetParameter(Parameter::TapLength, 1.0);
  reverb.SetParameter(Parameter::TapGain, 1.0);
  reverb.SetParameter(Parameter::TapDecay, 0.71000003814697266);
  reverb.SetParameter(Parameter::DiffusionEnabled, 1.0);
  reverb.SetParameter(Parameter::DiffusionStages, 1.0);
  reverb.SetParameter(Parameter::DiffusionDelay, 0.65999996662139893);
  reverb.SetParameter(Parameter::DiffusionFeedback, 0.76000010967254639);
  reverb.SetParameter(Parameter::LineCount, 1.0);
  reverb.SetParameter(Parameter::LineDelay, 0.9100002646446228);
  reverb.SetParameter(Parameter::LineDecay, 0.80999958515167236);
  reverb.SetParameter(Parameter::LateDiffusionEnabled, 1.0);
  reverb.SetParameter(Parameter::LateDiffusionStages, 1.0);
  reverb.SetParameter(Parameter::LateDiffusionDelay, 0.71499955654144287);
  reverb.SetParameter(Parameter::LateDiffusionFeedback, 0.71999979019165039);
  reverb.SetParameter(Parameter::PostLowShelfGain, 0.87999987602233887);
  reverb.SetParameter(Parameter::PostLowShelfFrequency, 0.19499993324279785);
  reverb.SetParameter(Parameter::PostHighShelfGain, 0.72000008821487427);
  reverb.SetParameter(Parameter::PostHighShelfFrequency, 0.520000159740448);
  reverb.SetParameter(Parameter::PostCutoffFrequency, 0.7150002121925354);
  reverb.SetParameter(Parameter::EarlyDiffusionModAmount, 0.41999998688697815);
  reverb.SetParameter(Parameter::EarlyDiffusionModRate, 0.30500012636184692);
  reverb.SetParameter(Parameter::LineModAmount, 0.4649999737739563);
  reverb.SetParameter(Parameter::LineModRate, 0.3199998140335083);
  reverb.SetParameter(Parameter::LateDiffusionModAmount, 0.40999993681907654);
  reverb.SetParameter(Parameter::LateDiffusionModRate, 0.31500011682510376);
  reverb.SetParameter(Parameter::TapSeed, 0.0003009999927598983);
  reverb.SetParameter(Parameter::DiffusionSeed, 0.00018899999849963933);
  reverb.SetParameter(Parameter::DelaySeed, 0.0001610000035725534);
  reverb.SetParameter(Parameter::PostDiffusionSeed, 0.00050099997315555811);
  reverb.SetParameter(Parameter::CrossSeed, 1.0);
  reverb.SetParameter(Parameter::DryOut, 0.0);
  reverb.SetParameter(Parameter::PredelayOut, 0.0);
  reverb.SetParameter(Parameter::EarlyOut, 0.0);
  reverb.SetParameter(Parameter::MainOut, 0.95499974489212036);
  reverb.SetParameter(Parameter::HiPassEnabled, 0.0);
  reverb.SetParameter(Parameter::LowPassEnabled, 1.0);
  reverb.SetParameter(Parameter::LowShelfEnabled, 0.0);
  reverb.SetParameter(Parameter::HighShelfEnabled, 0.0);
  reverb.SetParameter(Parameter::CutoffEnabled, 1.0);
  reverb.SetParameter(Parameter::LateStageTap, 1.0);
  reverb.SetParameter(Parameter::Interpolation, 1.0);


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

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    for (auto i = 2; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
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

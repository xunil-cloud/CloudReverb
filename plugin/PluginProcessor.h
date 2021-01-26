#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "../audio_engine/ReverbController.h"

//==============================================================================
class AudioPluginAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    AudioPluginAudioProcessor();
    ~AudioPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
	CloudSeed::ReverbController reverb;

  juce::AudioProcessorValueTreeState treeState;
  
  // Input
  juce::AudioParameterFloat* InputMix;
  juce::AudioParameterInt* PreDelay;
  juce::AudioParameterFloat* HighPass;
  juce::AudioParameterFloat* LowPass;

  // Early
  juce::AudioParameterInt* TapCount;
  juce::AudioParameterInt* TapLength;
  juce::AudioParameterFloat* TapGain;
  juce::AudioParameterFloat* TapDecay;

  juce::AudioParameterBool* DiffusionEnabled;
  juce::AudioParameterInt* DiffusionStages;
  juce::AudioParameterInt* DiffusionDelay;
  juce::AudioParameterFloat* DiffusionFeedback;
  juce::AudioParameterInt* LineCount;
  juce::AudioParameterFloat* LineDelay;
  juce::AudioParameterFloat* LineDecay;
  juce::AudioParameterBool* LateDiffusionEnabled;
  juce::AudioParameterInt* LateDiffusionStages;
  juce::AudioParameterInt* LateDiffusionDelay;
  juce::AudioParameterFloat* LateDiffusionFeedback;
  juce::AudioParameterFloat* PostLowShelfGain;
  juce::AudioParameterFloat* PostLowShelfFrequency;
  juce::AudioParameterFloat* PostHighShelfGain;
  juce::AudioParameterFloat* PostHighShelfFrequency;
  juce::AudioParameterFloat* PostCutoffFrequency;
  juce::AudioParameterFloat* EarlyDiffusionModAmount;
  juce::AudioParameterFloat* EarlyDiffusionModRate;
  juce::AudioParameterFloat* LineModAmount;
  juce::AudioParameterFloat* LineModRate;
  juce::AudioParameterFloat* LateDiffusionModAmount;
  juce::AudioParameterFloat* LateDiffusionModRate;
  juce::AudioParameterInt* TapSeed;
  juce::AudioParameterInt* DiffusionSeed;
  juce::AudioParameterInt* DelaySeed;
  juce::AudioParameterInt* PostDiffusionSeed;
  juce::AudioParameterFloat* CrossSeed;
  juce::AudioParameterFloat* DryOut;
  juce::AudioParameterFloat* PredelayOut;
  juce::AudioParameterFloat* EarlyOut;
  juce::AudioParameterFloat* MainOut;
  juce::AudioParameterBool* HiPassEnabled;
  juce::AudioParameterBool* LowPassEnabled;
  juce::AudioParameterBool* LowShelfEnabled;
  juce::AudioParameterBool* HighShelfEnabled;
  juce::AudioParameterBool* CutoffEnabled;
  juce::AudioParameterFloat* LateStageTap;
  juce::AudioParameterBool* Interpolation;
  
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessor)
};

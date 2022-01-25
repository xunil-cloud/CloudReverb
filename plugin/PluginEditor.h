#pragma once

#include "DiffusionBlock.h"
#include "LateDiffusionBlock.h"
#include "InputBlock.h"
#include "TapBlock.h"
#include "DelayBlock.h"
#include "Mixer.h"
#include "EqBlock.h"
#include "PluginProcessor.h"
#include <memory>

//==============================================================================
class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor &processorRef;
    MyLookAndFeel myLookAndFeel;
    juce::TextButton reset_button;
    std::unordered_map<juce::String, juce::Slider *> sliders;
    std::unordered_map<juce::String, juce::Label *> labels;
    juce::Grid input_sec;
    juce::Grid early_sec;
    std::vector<juce::SliderParameterAttachment *> attachments;
    InputBlock input;
    TapBlock tap;
    DelayBlock delay;
    Mixer mixer;
    EqBlock eq;
    // Block tap;
    // Block delayLine;
    DiffusionBlock diffusion1;
    LateDiffusionBlock diffusion2;
    // InputBlock input;
    // juce::AudioProcessorValueTreeState::ButtonAttachment a;

    std::unique_ptr<juce::ComponentBoundsConstrainer> limit;
    juce::Grid grid;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};

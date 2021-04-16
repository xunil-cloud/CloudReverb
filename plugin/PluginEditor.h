#pragma once

#include "PluginProcessor.h"

//==============================================================================
class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor {
public:
    explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& processorRef;
    juce::TextButton reset_button;
    juce::TextButton reset_button_;
    std::unordered_map<juce::String, juce::Slider*> sliders;
    std::unordered_map<juce::String, juce::Label*> labels;
    juce::Grid input_sec;
    juce::Grid early_sec;
    std::vector<juce::SliderParameterAttachment*> attachments;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};

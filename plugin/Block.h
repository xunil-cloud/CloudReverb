#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

#include <memory>
#include <unordered_map>

#include "MyLookAndFeel.h"
#include "ReverbSlider.h"

class Block : public juce::Component
{
public:
    // Block(const juce::String&, const juce::AudioProcessorValueTreeState&, const
    // std::vector<juce::String>&);
    Block(const juce::String &);
    void paint(juce::Graphics &) override;
    // void resized() override;
    void addParameter(const juce::String &, juce::RangedAudioParameter *param,
                      const ReverbSlider::Type type);
    void setupSeed(const juce::String &, juce::RangedAudioParameter *param);

protected:
    void layout();
    // const std::vector<juce::String> params_id{"InputMix"};
    const juce::String name;
    // std::unordered_map<juce::String, std::unique_ptr<juce::Slider>> sliders;

    std::vector<std::unique_ptr<ReverbSlider>> sliders;

    std::vector<std::unique_ptr<juce::Label>> labels;
    std::vector<std::unique_ptr<juce::SliderParameterAttachment>> attachments;
    juce::Slider seedSlider;
    std::unique_ptr<juce::SliderParameterAttachment> seedAttachment;
    // std::unordered_map<juce::String, std::unique_ptr<juce::Label>> labels;
    // std::unordered_map<juce::String, std::unique_ptr<juce::SliderParameterAttachment>>
    // attachments;
    //  NOTE: maybe we only need vector to store objects?
private:
    juce::FlexBox flex = juce::FlexBox();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Block)
};

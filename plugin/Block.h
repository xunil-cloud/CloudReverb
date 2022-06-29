#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

#include <memory>
#include <unordered_map>

#include "ReverbSlider.h"

class Block : public juce::Component
{
public:
    Block(const juce::String &);
    void paint(juce::Graphics &) override;
    void addParameter(const juce::String &, juce::RangedAudioParameter *param,
                      const ReverbSlider::Type type);
    void setupSeed(const juce::String &, juce::RangedAudioParameter *param);
    void setupNumberBoxSlider(juce::Slider &slider, juce::RangedAudioParameter *param);

protected:
    std::vector<std::unique_ptr<ReverbSlider>> sliders;

    std::vector<std::unique_ptr<juce::Label>> labels;
    std::vector<std::unique_ptr<juce::SliderParameterAttachment>> attachments;
    juce::Slider seedSlider;
    std::unique_ptr<juce::SliderParameterAttachment> seedAttachment;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Block)
};

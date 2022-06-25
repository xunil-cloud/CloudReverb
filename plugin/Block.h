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
    Block(const juce::String &);
    void paint(juce::Graphics &) override;
    void addParameter(const juce::String &, juce::RangedAudioParameter *param,
                      const ReverbSlider::Type type);
    void setupSeed(const juce::String &, juce::RangedAudioParameter *param);
    void setupNumberBoxSlider(juce::Slider &slider, juce::RangedAudioParameter *param);

protected:
    void layout();
    int getHeaderSize() { return getHeight() / 6.0 * header_ratio; };
    int getCenterY() { return (getY() + getBottom()) * 0.5; };
    int getCenterX() { return (getX() + getRight()) * 0.5; };

    const juce::String name;
    float header_ratio = 1.0;
    float x_ratio = 1.0;

    std::vector<std::unique_ptr<ReverbSlider>> sliders;

    std::vector<std::unique_ptr<juce::Label>> labels;
    std::vector<std::unique_ptr<juce::SliderParameterAttachment>> attachments;
    juce::Slider seedSlider;
    std::unique_ptr<juce::SliderParameterAttachment> seedAttachment;
    juce::FlexBox flex = juce::FlexBox();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Block)
};

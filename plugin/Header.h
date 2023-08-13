#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

#include "Block.h"
#include "CustomToggleButton.h"
#include "layout/Layout.h"

class Header : public Block
{
public:
    Header(const juce::String &, const juce::AudioProcessorValueTreeState &);
    void resized() override;
    void paint(juce::Graphics &) override;

private:
    CustomToggleButton interp_switch{CustomToggleButton::Style::circle, "interpolation"};
    std::unique_ptr<juce::ButtonParameterAttachment> attachment_interp_switch;
    ReverbSlider cross_seed_slider;
    std::unique_ptr<juce::SliderParameterAttachment> attachment_cross_seed;
    HeaderLayout layout;

public:
    juce::ComboBox combobox;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Header)
};

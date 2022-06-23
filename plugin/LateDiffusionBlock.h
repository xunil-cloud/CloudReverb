#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

#include <memory>
#include <unordered_map>

#include "Block.h"
#include "layout/ThreePartLayout.h"

class LateDiffusionBlock : public Block
{
public:
    LateDiffusionBlock(const juce::String &, const juce::AudioProcessorValueTreeState &);
    ~LateDiffusionBlock() override { button.setLookAndFeel(nullptr); }
    void resized() override;
    void paint(juce::Graphics &) override;

private:
    juce::ToggleButton button;
    std::unique_ptr<juce::ButtonParameterAttachment> button_attachment;
    MyLookAndFeel look;
    ThreePartLayout layout;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LateDiffusionBlock)
};

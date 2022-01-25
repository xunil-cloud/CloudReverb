#include "TapBlock.h"

TapBlock::TapBlock(const juce::String &name, const juce::AudioProcessorValueTreeState &state)
    : Block(name)
{
    using type = ReverbSlider::Type;
    addParameter("count", state.getParameter("TapCount"), type::Circle);
    addParameter("length", state.getParameter("TapLength"), type::Circle);
    addParameter("gain", state.getParameter("TapGain"), type::Circle);
    addParameter("decay", state.getParameter("TapDecay"), type::Circle);
}

void TapBlock::resized() { layout(); }

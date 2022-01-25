#include "EqBlock.h"

EqBlock::EqBlock(const juce::String &name, const juce::AudioProcessorValueTreeState &state)
    : Block(name)
{
    using type = ReverbSlider::Type;
    addParameter("low freq.", state.getParameter("PostLowShelfFrequency"), type::Circle);
    addParameter("high freq.", state.getParameter("PostHighShelfFrequency"), type::Circle);
    addParameter("low gain", state.getParameter("PostLowShelfGain"), type::Circle);
    addParameter("high gain", state.getParameter("PostHighShelfGain"), type::Circle);
    addParameter("cutoff", state.getParameter("PostCutoffFrequency"), type::Circle);
}

void EqBlock::resized() { layout(); }

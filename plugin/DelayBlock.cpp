#include "DelayBlock.h"

DelayBlock::DelayBlock(const juce::String &name, const juce::AudioProcessorValueTreeState &state)
    : Block(name)
{
    using type = ReverbSlider::Type;
    addParameter("delay", state.getParameter("LineDelay"), type::Circle);
    addParameter("decay", state.getParameter("LineDecay"), type::Circle);
    addParameter("mod amt.", state.getParameter("LineModAmount"), type::Circle);
    addParameter("mod rate", state.getParameter("LineModRate"), type::Circle);
}

void DelayBlock::resized() { layout(); }

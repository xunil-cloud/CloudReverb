#include "TapBlock.h"

TapBlock::TapBlock(const juce::String &name, const juce::AudioProcessorValueTreeState &state)
    : Block(name)
{
    using type = ReverbSlider::Type;
    addParameter("count", state.getParameter("TapCount"), type::Circle);
    addParameter("length", state.getParameter("TapLength"), type::Circle);
    addParameter("gain", state.getParameter("TapGain"), type::Circle);
    addParameter("decay", state.getParameter("TapDecay"), type::Circle);
    setupSeed("seed", state.getParameter("TapSeed"));
}

void TapBlock::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colour(0xff343434));
    layout.drawTitle(g, name, getLocalBounds());
    layout.drawTextUnderSlider(g, sliders[1].get(), getLocalBounds());
    layout.drawTextUnderSlider(g, sliders[2].get(), getLocalBounds());
    layout.drawTextUnderSlider(g, sliders[3].get(), getLocalBounds());
}
void TapBlock::resized()
{
    layout.placeUIs(sliders[0].get(), &seedSlider, sliders[1].get(), sliders[3].get(),
                    sliders[2].get(), getLocalBounds());
}

#include "TapBlock.h"

TapBlock::TapBlock(const juce::String &name, const juce::AudioProcessorValueTreeState &state)
    : Block(name)
{
    using type = ReverbSlider::Type;
    addParameter("length", state.getParameter("TapLength"), type::Circle);
    addParameter("gain", state.getParameter("TapGain"), type::Circle);
    addParameter("decay", state.getParameter("TapDecay"), type::Circle);
    setupSeed("seed", state.getParameter("TapSeed"));

    setupNumberBoxSlider(tapCount, state.getParameter("TapCount"));
    tapCount_attachment = std::make_unique<juce::SliderParameterAttachment>(
        *dynamic_cast<juce::RangedAudioParameter *>(state.getParameter("TapCount")), tapCount);
}

void TapBlock::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colour(0xff343434));
    layout.drawTitle(g, name, getLocalBounds());
    layout.drawTextUnderSlider(g, sliders[0].get(), getLocalBounds());
    layout.drawTextUnderSlider(g, sliders[1].get(), getLocalBounds());
    layout.drawTextUnderSlider(g, sliders[2].get(), getLocalBounds());
}
void TapBlock::resized()
{
    layout.placeUIs(&tapCount, &seedSlider, sliders[0].get(), sliders[2].get(), sliders[1].get(),
                    getLocalBounds());
}

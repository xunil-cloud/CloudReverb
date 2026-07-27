#include "DelayBlock.h"

DelayBlock::DelayBlock(const juce::String &name, const juce::AudioProcessorValueTreeState &state)
    : Block(name)
{
    using type = ReverbSlider::Type;
    addParameter("delay", state.getParameter("LineDelay"), type::Circle);
    addParameter("decay", state.getParameter("LineDecay"), type::Circle);
    addParameter("mod amt.", state.getParameter("LineModAmount"), type::Circle);
    addParameter("mod rate", state.getParameter("LineModRate"), type::Circle);
    setupSeed("seed", state.getParameter("DelaySeed"));

    auto enableParam = state.getParameter("LateDelayEnabled");
    enable_attachment = std::make_unique<juce::ButtonParameterAttachment>(*enableParam, enableButton);
    addAndMakeVisible(enableButton);
}

void DelayBlock::paint(juce::Graphics &g)
{
    Block::paint(g);
    layout.drawTitle(g, getName(), getLocalBounds());
    layout.drawButtonTitle(g, &enableButton, "ENABLE", getLocalBounds());
    layout.drawNumberBoxTitle(g, &seedSlider, "DELAY SEED", getLocalBounds());
    layout.drawTextUnderSlider(g, sliders[0].get(), getLocalBounds());
    layout.drawTextUnderSlider(g, sliders[1].get(), getLocalBounds());
    layout.drawTextRightToSlider(g, sliders[2].get(), getLocalBounds());
    layout.drawTextRightToSlider(g, sliders[3].get(), getLocalBounds());
}

void DelayBlock::resized()
{
    layout.placeUIs(nullptr, &seedSlider, sliders[0].get(), sliders[1].get(), sliders[2].get(),
                    sliders[3].get(), &enableButton, getLocalBounds());
}

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
    tapCount.setRange(1.0, 50.0, 1.0);
    tapCount_attachment = std::make_unique<juce::SliderParameterAttachment>(
        *state.getParameter("TapCount"), tapCount);

    auto enableParam = state.getParameter("TapEnabled");
    enable_attachment = std::make_unique<juce::ButtonParameterAttachment>(*enableParam, enableButton);
    addAndMakeVisible(enableButton);
}

void TapBlock::paint(juce::Graphics &g)
{
    Block::paint(g);
    layout.drawTitle(g, getName(), getLocalBounds());
    layout.drawButtonTitle(g, &enableButton, "ENABLE", getLocalBounds());
    layout.drawNumberBoxTitle(g, &tapCount, "TAPS COUNT", getLocalBounds());
    layout.drawNumberBoxTitle(g, &seedSlider, "TAP SEED", getLocalBounds());
    layout.drawTextUnderSlider(g, sliders[0].get(), getLocalBounds());
    layout.drawTextUnderSlider(g, sliders[1].get(), getLocalBounds());
    layout.drawTextUnderSlider(g, sliders[2].get(), getLocalBounds());
}
void TapBlock::resized()
{
    layout.placeUIs(&tapCount, &seedSlider, sliders[0].get(), sliders[2].get(), sliders[1].get(),
                    getLocalBounds());

    const float ratio = getWidth() / 540.f > getHeight() / 220.f
                            ? getHeight() / 220.f
                            : getWidth() / 540.f;
    const auto buttonSize = (int)(25 * ratio);
    enableButton.setBounds(getWidth() - (int)(20 * ratio) - buttonSize,
                           (int)(15 * ratio), buttonSize, buttonSize);
}

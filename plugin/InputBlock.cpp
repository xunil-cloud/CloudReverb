#include "InputBlock.h"

InputBlock::InputBlock(const juce::String &name, const juce::AudioProcessorValueTreeState &state)
    : Block(name)
{
    using type = ReverbSlider::Type;
    addParameter("blend", state.getParameter("InputMix"), type::Circle);
    addParameter("predelay", state.getParameter("PreDelay"), type::Circle);
    addParameter("low cut", state.getParameter("HighPass"), type::Circle);
    addParameter("high cut", state.getParameter("LowPass"), type::Circle);

    auto param = state.getParameter("LowPassEnabled");
    LowButton_attachment = std::make_unique<juce::ButtonParameterAttachment>(
        *dynamic_cast<juce::RangedAudioParameter *>(param), lowButton);

    auto param_ = state.getParameter("HiPassEnabled");
    HighButton_attachment = std::make_unique<juce::ButtonParameterAttachment>(
        *dynamic_cast<juce::RangedAudioParameter *>(param_), HighButton);

    addAndMakeVisible(lowButton);
    addAndMakeVisible(HighButton);
}

void InputBlock::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colour(0xff343434));
    layout.drawTitle(g, name, getLocalBounds());
    layout.drawTextUnderSlider(g, sliders[0].get(), getLocalBounds());
    layout.drawTextUnderSlider(g, sliders[1].get(), getLocalBounds());
    layout.drawTextUnderSlider(g, sliders[2].get(), getLocalBounds());
    layout.drawTextUnderSlider(g, sliders[3].get(), getLocalBounds());
}
void InputBlock::resized()
{
    layout.placeUIs(sliders[0].get(), sliders[1].get(), sliders[2].get(), sliders[3].get(),
                    &lowButton, &HighButton, getLocalBounds());
}

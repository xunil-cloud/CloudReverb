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

void InputBlock::resized()
{
    layout();
    auto y1 = sliders[2]->getY() * 0.8;
    auto y2 = sliders[3]->getY() * 0.8;
    auto size = sliders[2]->getWidth();
    auto buttonSize = std::min(30.0, size * 0.40);
    lowButton.setSize(buttonSize, buttonSize);
    lowButton.setCentrePosition(sliders[3]->getRight(), y2);
    HighButton.setSize(buttonSize, buttonSize);
    HighButton.setCentrePosition(sliders[2]->getRight(), y1);
}

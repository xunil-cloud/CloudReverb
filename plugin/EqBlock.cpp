#include "EqBlock.h"

EqBlock::EqBlock(const juce::String &name, const juce::AudioProcessorValueTreeState &state)
    : Block(name)
{
    using type = ReverbSlider::Type;
    addParameter("low freq.", state.getParameter("PostLowShelfFrequency"), type::Circle);
    addParameter("low gain", state.getParameter("PostLowShelfGain"), type::Circle);
    addParameter("high gain", state.getParameter("PostHighShelfGain"), type::Circle);
    addParameter("high freq.", state.getParameter("PostHighShelfFrequency"), type::Circle);
    addParameter("cutoff", state.getParameter("PostCutoffFrequency"), type::Circle);

    auto param = state.getParameter("LowShelfEnabled");
    LowButton_attachment = std::make_unique<juce::ButtonParameterAttachment>(
        *dynamic_cast<juce::RangedAudioParameter *>(param), lowButton);
    addAndMakeVisible(lowButton);

    auto param_ = state.getParameter("HighShelfEnabled");
    HighButton_attachment = std::make_unique<juce::ButtonParameterAttachment>(
        *dynamic_cast<juce::RangedAudioParameter *>(param_), HighButton);
    addAndMakeVisible(HighButton);

    param_ = state.getParameter("CutoffEnabled");
    cutoffButton_attachment = std::make_unique<juce::ButtonParameterAttachment>(
        *dynamic_cast<juce::RangedAudioParameter *>(param_), cutoffButton);
    addAndMakeVisible(cutoffButton);
}
void EqBlock::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colour(0xff343434));
    layout.drawTitle(g, getName(), getLocalBounds());
    layout.drawTextUnderSlider(g, sliders[0].get(), getLocalBounds(), 7);
    layout.drawTextUnderSlider(g, sliders[1].get(), getLocalBounds(), 7);
    layout.drawTextUnderSlider(g, sliders[2].get(), getLocalBounds(), 7);
    layout.drawTextUnderSlider(g, sliders[3].get(), getLocalBounds(), 7);
    layout.drawTextUnderSlider(g, sliders[4].get(), getLocalBounds(), 7);
    layout.drawLines(g, getLocalBounds());
}

void EqBlock::resized()
{
    layout.placeUIs(sliders[0].get(), sliders[1].get(), sliders[3].get(), sliders[2].get(),
                    sliders[4].get(), &lowButton, &HighButton, &cutoffButton, getLocalBounds());
}

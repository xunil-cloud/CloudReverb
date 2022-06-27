#include "LateDiffusionBlock.h"

LateDiffusionBlock::LateDiffusionBlock(const juce::String &name,
                                       const juce::AudioProcessorValueTreeState &state)
    : Block(name)
{
    using type = ReverbSlider::Type;
    addParameter("delay", state.getParameter("LateDiffusionDelay"), type::Circle);
    addParameter("feedback", state.getParameter("LateDiffusionFeedback"), type::Circle);
    addParameter("mod amt.", state.getParameter("LateDiffusionModAmount"), type::Circle);
    addParameter("mod rate", state.getParameter("LateDiffusionModRate"), type::Circle);
    setupSeed("seed", state.getParameter("PostDiffusionSeed"));

    setupNumberBoxSlider(stages, state.getParameter("LateDiffusionStages"));
    stages_attachment = std::make_unique<juce::SliderParameterAttachment>(
        *dynamic_cast<juce::RangedAudioParameter *>(state.getParameter("LateDiffusionStages")),
        stages);

    auto param = state.getParameter("LateDiffusionEnabled");
    button_attachment = std::make_unique<juce::ButtonParameterAttachment>(
        *dynamic_cast<juce::RangedAudioParameter *>(param), button);
    button.setLookAndFeel(&look);
    addAndMakeVisible(button);
}

void LateDiffusionBlock::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colour(0xff343434));
    layout.drawTitle(g, name, getLocalBounds());
    layout.drawTextUnderSlider(g, sliders[0].get(), getLocalBounds());
    layout.drawTextUnderSlider(g, sliders[1].get(), getLocalBounds());
    layout.drawTextRightToSlider(g, sliders[2].get(), getLocalBounds());
    layout.drawTextRightToSlider(g, sliders[3].get(), getLocalBounds());
}
void LateDiffusionBlock::resized()
{
    layout.placeUIs(&stages, &seedSlider, sliders[0].get(), sliders[1].get(), sliders[2].get(),
                    sliders[3].get(), &button, getLocalBounds());
}

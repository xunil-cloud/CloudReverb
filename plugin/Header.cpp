#include "Header.h"

Header::Header(const juce::String &name, const juce::AudioProcessorValueTreeState &state)
    : Block(name), cross_seed_slider("cross seed", ReverbSlider::Type::Circle)
{
    setInterceptsMouseClicks(false, true);
    using type = ReverbSlider::Type;
    // addParameter("stages", state.getParameter("DiffusionStages"), type::Circle);

    // interp_switch.setButtonText("interppolation");
    interp_switch.setName("interpolation");
    auto param = state.getParameter("Interpolation");
    attachment_interp_switch = std::make_unique<juce::ButtonParameterAttachment>(
        *dynamic_cast<juce::RangedAudioParameter *>(param), interp_switch);

    param = state.getParameter("CrossSeed");
    attachment_cross_seed = std::make_unique<juce::SliderParameterAttachment>(
        *dynamic_cast<juce::RangedAudioParameter *>(param), cross_seed_slider);

    cross_seed_slider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(interp_switch);
    addAndMakeVisible(cross_seed_slider);
}
void Header::paint(juce::Graphics &g)
{
    // g.fillAll(juce::Colour(0xff343434));
    layout.drawTitle(g, name, getLocalBounds());
    layout.drawTextRightToSlider(g, &cross_seed_slider, getLocalBounds());
    layout.drawTextRightToSlider(g, &interp_switch, getLocalBounds());
}

void Header::resized()
{
    // button.setSize(1, 1);
    // button.setBoundsToFit(getLocalBounds()
    //                           .withSizeKeepingCentre(getWidth() * 0.95, getHeight() * 0.9)
    //                           .removeFromTop(getHeaderSize())
    //                           .withSizeKeepingCentre(getWidth() * 0.95, getHeaderSize() * 0.7),
    //                       juce::Justification::centredRight, false);
    // layout();
    layout.placeUIs(&cross_seed_slider, &interp_switch, getLocalBounds());
}

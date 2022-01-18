#include "DiffusionBlock.h"

DiffusionBlock::DiffusionBlock(const juce::String& name, const juce::AudioProcessorValueTreeState& state) : Block(name) {
    using type = ReverbSlider::Type;
    addParameter("stages", state.getParameter("DiffusionStages"), type::Circle);
    addParameter("delay", state.getParameter("DiffusionDelay"), type::Circle);
    addParameter("feedback", state.getParameter("DiffusionFeedback"), type::Circle);
    addParameter("mod amt.", state.getParameter("EarlyDiffusionModAmount"), type::Circle);
    addParameter("mod rate", state.getParameter("EarlyDiffusionModRate"), type::Circle);
    //button.setToggleState();
    auto param = state.getParameter("DiffusionEnabled");
    button_attachment = std::make_unique<juce::ButtonParameterAttachment>(*dynamic_cast<juce::RangedAudioParameter*>(param), button);
    button.setLookAndFeel(&look);
    addAndMakeVisible(button);
}

void DiffusionBlock::resized() {
    button.setBounds(getWidth() - 35, 2, 30, 30);
    auto main = getLocalBounds();
    main.removeFromTop(50);
    auto size = 50;
    auto flex = juce::FlexBox();
    using Fl = juce::FlexBox;
    flex.justifyContent = Fl::JustifyContent::spaceAround;
    //flex.alignItems = Fl::AlignItems::stretch;
    flex.alignContent = Fl::AlignContent::center;
    //flex.alignContent = Fl::AlignContent::stretch;
    auto items = juce::Array<juce::FlexItem>();

    for (auto& i : sliders) {
        auto slider = i.get();
        items.add(juce::FlexItem(size, size, *slider));
    }
    flex.items = items;
    flex.performLayout(main);

    for (size_t i = 0; i < sliders.size(); i++) {
        auto& slider = sliders[i];
        auto& label = labels[i];
        label->setCentrePosition(slider->getBounds().getCentreX(), slider->getBounds().getBottom() + 8);
    }
}

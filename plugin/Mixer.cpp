#include "Mixer.h"

Mixer::Mixer(const juce::String& name, const juce::AudioProcessorValueTreeState& state) : Block(name) {
    using type = ReverbSlider::Type;
    addParameter("dry", state.getParameter("DryOut"), type::Line);
    addParameter("predelay", state.getParameter("PredelayOut"), type::Line);
    addParameter("early", state.getParameter("EarlyOut"), type::Line);
    addParameter("late", state.getParameter("MainOut"), type::Line);
}

void Mixer::resized() {
    auto main = getLocalBounds();
    main.removeFromTop(50);
    main.removeFromBottom(30);
    auto size = 50;
    auto flex = juce::FlexBox();
    using Fl = juce::FlexBox;
    flex.justifyContent = Fl::JustifyContent::spaceAround;
    flex.alignItems = Fl::AlignItems::stretch;
    flex.alignContent = Fl::AlignContent::center;
    flex.alignContent = Fl::AlignContent::stretch;
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

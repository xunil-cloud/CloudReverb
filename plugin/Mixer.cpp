#include "Mixer.h"

Mixer::Mixer(const juce::String &name, const juce::AudioProcessorValueTreeState &state)
    : Block(name)
{
    using type = ReverbSlider::Type;
    addParameter("dry", state.getParameter("DryOut"), type::Line);
    addParameter("predelay", state.getParameter("PredelayOut"), type::Line);
    addParameter("early", state.getParameter("EarlyOut"), type::Line);
    addParameter("late", state.getParameter("MainOut"), type::Line);
    x_ratio = 1.5;
}

void Mixer::resized()
{
    auto main = getLocalBounds();
    main.removeFromTop(getHeight() / 7.0 * header_ratio);
    auto rec =
        getLocalBounds()
            // .withSizeKeepingCentre(getWidth() * 0.9, getHeight() * ((1/header_ratio-1)*
            // 0.05+0.9));
            .withSizeKeepingCentre(getWidth() * x_ratio * (0.9 + 1 / x_ratio - 1),
                                   getHeight() * header_ratio * (0.9 + 1 / header_ratio - 1));
    rec.removeFromTop(getHeaderSize());
    // auto size = std::min(getWidth() / 4.0 * 0.3, getHeight() * 0.3);
    auto size = getHeight() * 0.3;
    seedSlider.setBounds(30, 2, getWidth() / 5.0, getHeight() / 7.0);

    flex.items.clearQuick();
    for (auto &i : sliders)
    {
        auto slider = i.get();
        flex.items.add(
            juce::FlexItem(size * 0.6, size, *slider).withMaxHeight(70).withMaxWidth(70));
    }
    auto bound = main.withSizeKeepingCentre(main.getWidth() * 0.8, main.getHeight() * 0.8);
    flex.performLayout(rec);

    for (auto &i : sliders)
    {
        auto slider = i.get();
        auto bound = slider->getBounds();
        bound = bound.withTop(bound.getY() - size * 0.5);
        slider->setBounds(bound);
    }

    rec = getLocalBounds()
              // .withSizeKeepingCentre(getWidth() * 0.9, getHeight() * ((1/header_ratio-1)*
              // 0.05+0.9));
              .withSizeKeepingCentre(getWidth() * 0.9,
                                     getHeight() * header_ratio * (0.9 + 1 / header_ratio - 1));
    rec.removeFromTop(getHeaderSize());
    for (size_t i = 0; i < sliders.size(); i++)
    {
        auto &slider = sliders[i];
        auto &label = labels[i];
        label->setFont(juce::Font("Roboto", 20, juce::Font::plain));
        auto fontsize = std::min(rec.getWidth() * 0.05 * x_ratio, size * 0.35);
        label->setFont(juce::Font("Roboto", fontsize, juce::Font::plain));
        // label->setFont(juce::Font("Open Sans Condensed", 20, juce::Font::plain));
        // label->setSize(bound.getWidth() / 4.0, (bound.getHeight() - slider->getHeight()) / 2.0);
        // label->setCentrePosition(slider->getBounds().getCentreX(),
        //                          getHeight() - label->getHeight() * 0.5);
        // label->setTopLeftPosition(label->getX(), bound.getBottom() - 1.2 * label->getHeight());
        label->setSize(float(rec.getWidth()) / sliders.size(), fontsize + 2);
        label->setCentrePosition(slider->getBounds().getCentreX(),
                                 slider->getBounds().getBottom() + fontsize * 0.75);
    }
    /*

    auto main = getLocalBounds();
    auto rec =
        getLocalBounds()
            // .withSizeKeepingCentre(getWidth() * 0.9, getHeight() * ((1/header_ratio-1)*
            // 0.05+0.9));
            .withSizeKeepingCentre(getWidth() * x_ratio * (0.9 + 1 / x_ratio - 1),
                                   getHeight() * header_ratio * (0.9 + 1 / header_ratio - 1));
    rec.removeFromTop(getHeaderSize());
    main.removeFromTop(50);
    main.removeFromBottom(30);
    auto size = 50;
    size = getHeight() * 0.3;

    auto flex = juce::FlexBox();
    using Fl = juce::FlexBox;
    flex.justifyContent = Fl::JustifyContent::spaceAround;
    flex.alignItems = Fl::AlignItems::stretch;
    flex.alignContent = Fl::AlignContent::center;
    flex.alignContent = Fl::AlignContent::stretch;
    auto items = juce::Array<juce::FlexItem>();

    for (auto &i : sliders)
    {
        auto slider = i.get();
        items.add(juce::FlexItem(size, size, *slider));
    }
    flex.items = items;
    flex.performLayout(main);

    for (size_t i = 0; i < sliders.size(); i++)
    {
        auto &slider = sliders[i];
        auto &label = labels[i];
        // label->setCentrePosition(slider->getBounds().getCentreX(),
        //                          slider->getBounds().getBottom() + 8);
        label->setFont(juce::Font("Roboto", 20, juce::Font::plain));
        auto fontsize = std::min(rec.getWidth() * 0.05 * x_ratio, size * 0.35);
        label->setFont(juce::Font("Roboto", fontsize, juce::Font::plain));
        // label->setFont(juce::Font("Open Sans Condensed", 20, juce::Font::plain));
        // label->setSize(bound.getWidth() / 4.0, (bound.getHeight() - slider->getHeight()) / 2.0);
        // label->setCentrePosition(slider->getBounds().getCentreX(),
        //                          getHeight() - label->getHeight() * 0.5);
        // label->setTopLeftPosition(label->getX(), bound.getBottom() - 1.2 * label->getHeight());
        label->setSize(float(rec.getWidth()) / sliders.size(), fontsize + 2);
        label->setCentrePosition(slider->getBounds().getCentreX(),
                                 slider->getBounds().getBottom() + fontsize * 0.75);
    }
    */
}

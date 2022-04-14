#include "Block.h"

Block::Block(const juce::String &name) : name(std::move(name))
{
    using Fl = juce::FlexBox;
    flex.justifyContent = Fl::JustifyContent::spaceBetween;
    flex.alignContent = Fl::AlignContent::center;
    flex.alignItems = Fl::AlignItems::center;
    flex.items = juce::Array<juce::FlexItem>();
}
void Block::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colour(0xff343434));
    g.setColour(juce::Colour(0xffd6d6d6));

    auto rec = getLocalBounds()
                   .withSizeKeepingCentre(getWidth() * 0.9,
                                          getHeight() * header_ratio * (0.9 + 1 / header_ratio - 1))
                   .removeFromTop(getHeaderSize());
    g.setFont(juce::Font(std::min(36, rec.getHeight())));
    g.drawFittedText(name, rec, juce::Justification::centred, 1);
}

void Block::setupSeed(const juce::String &name, juce::RangedAudioParameter *param)
{
    seedSlider.setName(name);
    addAndMakeVisible(seedSlider);
    seedSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    seedSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xff343434));

    seedAttachment = std::make_unique<juce::SliderParameterAttachment>(
        *dynamic_cast<juce::RangedAudioParameter *>(param), seedSlider);
}
void Block::addParameter(const juce::String &name, juce::RangedAudioParameter *param,
                         ReverbSlider::Type type)
{

    std::unique_ptr<ReverbSlider> slider = std::make_unique<ReverbSlider>(name, type);
    slider->setPopupDisplayEnabled(true, true, this);

    auto attachment = std::make_unique<juce::SliderParameterAttachment>(
        *dynamic_cast<juce::RangedAudioParameter *>(param), *slider.get());
    attachments.push_back(std::move(attachment));

    auto label = std::make_unique<juce::Label>(param->paramID, name);
    label->setMinimumHorizontalScale(1);
    label->setJustificationType(juce::Justification::centred);
    addAndMakeVisible(slider.get());
    addAndMakeVisible(label.get());
    sliders.push_back(std::move(slider));
    labels.push_back(std::move(label));
}

void Block::layout()
{
    auto main = getLocalBounds();
    main.removeFromTop(getHeight() / 7.0 * header_ratio);
    auto rec = getLocalBounds().withSizeKeepingCentre(
        getWidth() * x_ratio * (0.9 + 1 / x_ratio - 1),
        getHeight() * header_ratio * (0.9 + 1 / header_ratio - 1));
    rec.removeFromTop(getHeaderSize());

    auto size = getHeight() * 0.3;
    seedSlider.setBounds(30, 10, getWidth() / 7.0, 25);

    flex.items.clearQuick();
    for (auto &i : sliders)
    {
        auto slider = i.get();
        flex.items.add(juce::FlexItem(size, size, *slider).withMaxHeight(70).withMaxWidth(70));
    }
    flex.performLayout(rec);

    rec = getLocalBounds().withSizeKeepingCentre(
        getWidth() * 0.9, getHeight() * header_ratio * (0.9 + 1 / header_ratio - 1));
    rec.removeFromTop(getHeaderSize());

    for (size_t i = 0; i < sliders.size(); i++)
    {
        auto &slider = sliders[i];
        auto &label = labels[i];
        auto fontsize = std::min(rec.getWidth() * 0.05 * x_ratio, size * 0.35);
        label->setFont(juce::Font("Roboto", fontsize, juce::Font::plain));
        label->setSize(float(rec.getWidth()) / sliders.size(), fontsize + 2);
        label->setCentrePosition(slider->getBounds().getCentreX(),
                                 slider->getBounds().getBottom() + fontsize * 0.75);
    }
}

#include "Block.h"

Block::Block(const juce::String &name) : name(std::move(name))
{
    using Fl = juce::FlexBox;
    flex.justifyContent = Fl::JustifyContent::spaceBetween;
    flex.alignContent = Fl::AlignContent::center;
    flex.items = juce::Array<juce::FlexItem>();
}
void Block::paint(juce::Graphics &g)
{
    // g.fillAll(juce::Colour(0xff283338));
    g.fillAll(juce::Colour(0xff383838));
    // g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.setFont(juce::Font(24));
    g.setColour(juce::Colour(0xffd4be98));
    g.setColour(juce::Colour(0xffd6d6d6));
    g.drawText(name, 0, 0, getWidth(), 50, juce::Justification::centred);
}
/*
void Block::resized() {
    return;
    auto top = getLocalBounds();
    auto main_1 = top.removeFromBottom(200);
    auto main_2 = main_1.removeFromBottom(main_1.getHeight() / 2);
    auto const padding = 15;
    auto const size = 70;
    // NOTE: we do fix size just for now.

    sliders[params_id[0]]->setBounds(padding, main_1.getY() + 5, size, size);
    sliders[params_id[1]]->setBounds(padding * 3 + size, main_1.getY() + 5, size, size);
    sliders[params_id[2]]->setBounds(padding, main_2.getY() + 5, size, size);
    sliders[params_id[3]]->setBounds(padding * 3 + size, main_2.getY() + 5, size, size);
    for (auto i : params_id) {
        auto slider = sliders[i].get();
        auto label = labels[i].get();
        label->setCentrePosition(slider->getBounds().getCentreX(), slider->getBounds().getBottom() +
8);
    }
}
*/
void Block::setupSeed(const juce::String &name, juce::RangedAudioParameter *param)
{
    seedSlider.setName(name);
    seedSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    addAndMakeVisible(seedSlider);
    seedSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    seedAttachment = std::make_unique<juce::SliderParameterAttachment>(
        *dynamic_cast<juce::RangedAudioParameter *>(param), seedSlider);
}
void Block::addParameter(const juce::String &name, juce::RangedAudioParameter *param,
                         ReverbSlider::Type type)
{
    auto const fontSize = 16;

    std::unique_ptr<ReverbSlider> slider = std::make_unique<ReverbSlider>(name, type);
    // sliders.insert({name, std::move(slider)});
    slider->setPopupDisplayEnabled(true, true, this);

    auto attachment = std::make_unique<juce::SliderParameterAttachment>(
        *dynamic_cast<juce::RangedAudioParameter *>(param), *slider.get());
    attachments.push_back(std::move(attachment));

    auto label = std::make_unique<juce::Label>(param->paramID, name);
    label->setFont(juce::Font(fontSize));
    label->setColour(juce::Label::ColourIds::textColourId, juce::Colour(0xffd4be98));
    label->setColour(juce::Label::ColourIds::textColourId, juce::Colour(0xffd6d6d6));
    // label->setColour(juce::Label::ColourIds::backgroundColourId,
    // juce::Colour(juce::Colours::antiquewhite));
    label->setMinimumHorizontalScale(1);
    auto const width = label->getFont().getStringWidthFloat(label->getText()) +
                       label->getBorderSize().getLeftAndRight() + 1;
    auto const height = fontSize + label->getBorderSize().getTopAndBottom() + 1;
    label->setSize(width, height);
    label->setJustificationType(juce::Justification::centred);
    // labels.insert({name, std::move(label)});
    addAndMakeVisible(slider.get());
    addAndMakeVisible(label.get());
    sliders.push_back(std::move(slider));
    labels.push_back(std::move(label));
}

void Block::layout()
{
    auto main = getLocalBounds();
    main.removeFromTop(getHeight() / 7.0);
    // auto size = std::min(getWidth() / 4.0 * 0.3, getHeight() * 0.3);
    auto size = getHeight() * 0.3;
    seedSlider.setBounds(30, 2, getWidth() / 5.0, getHeight() / 7.0);

    flex.items.clearQuick();
    for (auto &i : sliders)
    {
        auto slider = i.get();
        flex.items.add(juce::FlexItem(size, size, *slider).withMaxHeight(70).withMaxWidth(70));
    }
    auto bound = main.withSizeKeepingCentre(main.getWidth() * 0.8, main.getHeight() * 0.8);
    flex.performLayout(bound);

    for (size_t i = 0; i < sliders.size(); i++)
    {
        auto &slider = sliders[i];
        auto &label = labels[i];
        label->setFont(juce::Font("Roboto", 20, juce::Font::plain));
        // label->setFont(juce::Font("Open Sans Condensed", 20, juce::Font::plain));
        label->setSize(bound.getWidth() / 4.0, (bound.getHeight() - slider->getHeight()) / 2.0);
        label->setCentrePosition(slider->getBounds().getCentreX(),
                                 getHeight() - label->getHeight() * 0.5);
        label->setTopLeftPosition(label->getX(), bound.getBottom() - 1.2 * label->getHeight());
    }
}

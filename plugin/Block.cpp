#include "Block.h"

Block::Block(const juce::String &name) : name(std::move(name)) {}
void Block::paint(juce::Graphics &g)
{
    g.drawRect(this->getLocalBounds(), 2);
    g.setFont(juce::Font(24));
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
    // label->setColour(juce::Label::ColourIds::textColourId, juce::Colour(0xffd4be98));
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

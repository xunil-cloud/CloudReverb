#include "Block.h"

Block::Block(const juce::String &name) : juce::Component(name) {}
void Block::paint(juce::Graphics &g) { g.fillAll(juce::Colour(0xff343434)); }

void Block::setupNumberBoxSlider(juce::Slider &slider, juce::RangedAudioParameter *param)
{
    addAndMakeVisible(slider);
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    slider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::transparentBlack);
    slider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId,
                     juce::Colours::transparentBlack);
}
void Block::setupSeed(const juce::String &name, juce::RangedAudioParameter *param)
{
    seedSlider.setName(name);
    addAndMakeVisible(seedSlider);
    seedSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    seedSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::transparentBlack);
    seedSlider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId,
                         juce::Colours::transparentBlack);

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

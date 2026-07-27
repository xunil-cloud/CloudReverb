#include "Block.h"
#include "MyLookAndFeel.h"

Block::Block(const juce::String &name) : juce::Component(name) { setOpaque(false); }
void Block::paint(juce::Graphics &g)
{
    auto bounds = getLocalBounds().toFloat().reduced(2.0f);
    bool isLight = MyLookAndFeel::isLightMode;

    if (isLight)
    {
        // Sleek Interface light card: darker soft-slate surface with clean depth
        juce::Colour gradientStart(0xffd8e0ea);
        juce::Colour gradientEnd(0xffc5cfdd);
        juce::ColourGradient gradient(gradientStart, bounds.getX(), bounds.getY(),
                                      gradientEnd, bounds.getX(), bounds.getBottom(), false);
        g.setGradientFill(gradient);
        g.fillRoundedRectangle(bounds, 10.0f);

        // Softer clean shadow (bottom edge glow)
        auto shadowBounds = bounds.translated(0, 2.0f);
        juce::ColourGradient shadow(juce::Colours::black.withAlpha(0.065f),
                                    shadowBounds.getBottomLeft(),
                                    juce::Colours::black.withAlpha(0.0f),
                                    shadowBounds.getBottomLeft().translated(0, -7.0f), false);
        g.setGradientFill(shadow);
        g.fillRoundedRectangle(shadowBounds, 10.0f);

        // Re-draw card on top of shadow
        g.setGradientFill(gradient);
        g.fillRoundedRectangle(bounds, 10.0f);

        // Softer sleek border for a cleaner light theme frame
        g.setColour(juce::Colour(0xff9aabba).withAlpha(0.72f));
        g.drawRoundedRectangle(bounds, 10.0f, 1.05f);

        // Very subtle inner highlight line (top edge)
        g.setColour(juce::Colour(0xfff7faff).withAlpha(0.30f));
        juce::Path topLine;
        topLine.addRoundedRectangle(bounds.getX() + 10.0f, bounds.getY(),
                                    bounds.getWidth() - 20.0f, 1.5f, 0.75f);
        g.fillPath(topLine);
    }
    else
    {
        juce::Colour gradientStart(0xff1c2738); // Dark Navy Card
        juce::Colour gradientEnd(0xff141d2a);
        juce::ColourGradient gradient(gradientStart, bounds.getTopLeft(), gradientEnd, bounds.getBottomRight(), false);
        g.setGradientFill(gradient);
        g.fillRoundedRectangle(bounds, 10.0f);
        
        g.setColour(juce::Colour(0xff2e3e54).withAlpha(0.85f)); // Dark Navy border
        g.drawRoundedRectangle(bounds, 10.0f, 1.25f);
    }
}

void Block::setupNumberBoxSlider(juce::Slider &slider, juce::RangedAudioParameter *param)
{
    addAndMakeVisible(slider);
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical); // Vertical drag
    slider.setMouseDragSensitivity(600); // 1/4 precise drag speed!
    slider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::transparentBlack);
    slider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId,
                     juce::Colours::transparentBlack);
}
void Block::setupSeed(const juce::String &name, juce::RangedAudioParameter *param)
{
    seedSlider.setName(name);
    addAndMakeVisible(seedSlider);
    seedSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical); // Vertical drag
    seedSlider.setMouseDragSensitivity(2400); // 1/4 of previous speed (ultra-precise drag!)
    seedSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::transparentBlack);
    seedSlider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId,
                         juce::Colours::transparentBlack);

    seedAttachment = std::make_unique<juce::SliderParameterAttachment>(*param, seedSlider);
}
void Block::addParameter(const juce::String &name, juce::RangedAudioParameter *param,
                         ReverbSlider::Type type)
{

    std::unique_ptr<ReverbSlider> slider = std::make_unique<ReverbSlider>(name, type);
    slider->setPopupDisplayEnabled(true, true, this);

    auto attachment = std::make_unique<juce::SliderParameterAttachment>(*param, *slider.get());
    attachments.push_back(std::move(attachment));

    auto label = std::make_unique<juce::Label>(param->paramID, name);
    label->setMinimumHorizontalScale(1);
    label->setJustificationType(juce::Justification::centred);
    addAndMakeVisible(slider.get());
    addAndMakeVisible(label.get());
    sliders.push_back(std::move(slider));
    labels.push_back(std::move(label));
}

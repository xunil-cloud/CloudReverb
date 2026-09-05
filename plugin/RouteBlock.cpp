#include "RouteBlock.h"
#include "BinaryData.h"
#include <memory>

RouteBlock::RouteBlock(const juce::String &name, const juce::AudioProcessorValueTreeState &state)
    : Block(name)
{

    std::unique_ptr<juce::XmlElement> svg_xml(juce::XmlDocument::parse(BinaryData::diagram_svg));
    diagram = juce::Drawable::createFromImageData(BinaryData::diagram_svg, BinaryData::diagram_svgSize);
    diagram_component = std::make_unique<juce::DrawableComponent>(*diagram);

    mode_switch.setButtonText("pre/post");
    mode_switch.setName("pre / post");
    auto param = state.getParameter("LateStageTap");
    attachment_mode_switch = std::make_unique<juce::ButtonParameterAttachment>(
        *dynamic_cast<juce::RangedAudioParameter *>(param), mode_switch);
    addAndMakeVisible(&mode_switch);
    if (diagram)
    {
        addAndMakeVisible(diagram_component.get());
    }
    setupNumberBoxSlider(lineCount, state.getParameter("LineCount"));
    lineCount_attachment = std::make_unique<juce::SliderParameterAttachment>(
        *dynamic_cast<juce::RangedAudioParameter *>(state.getParameter("LineCount")), lineCount);

    mode_switch.onStateChange = [this]() {
        this->mode = mode_switch.getToggleState() ? RouteBlock::Mode::POST : RouteBlock::Mode::PRE;
        repaint();
    };
}

void RouteBlock::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colour(0xff343434));
    layout.drawTextRightToSlider(g, &mode_switch, getLocalBounds());
    layout.drawTitle(g, "Route", getLocalBounds());
}

void RouteBlock::paintOverChildren(juce::Graphics &g)
{

    if (mode == RouteBlock::Mode::PRE)
    {
        layout.drawTextOndiagram(g, "late delay", "late diffusion", getLocalBounds());
    }
    else
    {
        layout.drawTextOndiagram(g, "late diffusion", "late delay", getLocalBounds());
    }
}

void RouteBlock::resized()
{
    layout.placeUIs(diagram_component.get(), &lineCount, &mode_switch, getLocalBounds());
}

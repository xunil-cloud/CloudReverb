#include "RouteBlock.h"
#include "BinaryData.h"

RouteBlock::RouteBlock(const juce::String &name, const juce::AudioProcessorValueTreeState &state)
    : Block(name)
{

    std::unique_ptr<juce::XmlElement> svg_xml(juce::XmlDocument::parse(BinaryData::diagram_svg));
    diagram = juce::Drawable::createFromSVG(*svg_xml);

    mode_switch.setButtonText("pre/post");
    mode_switch.setName("pre / post");
    auto param = state.getParameter("LateStageTap");
    attachment_mode_switch = std::make_unique<juce::ButtonParameterAttachment>(
        *dynamic_cast<juce::RangedAudioParameter *>(param), mode_switch);
    addAndMakeVisible(&mode_switch);
    if (diagram)
    {
        addAndMakeVisible(diagram.get());
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
    layout.placeUIs(diagram.get(), &lineCount, &mode_switch, getLocalBounds());
}

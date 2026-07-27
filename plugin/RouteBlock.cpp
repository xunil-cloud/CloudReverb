#include "RouteBlock.h"
#include "BinaryData.h"

RouteBlock::RouteBlock(const juce::String &name, const juce::AudioProcessorValueTreeState &state)
    : Block(name)
{
    mode_switch.setButtonText("pre/post");
    mode_switch.setName("pre / post");
    auto param = state.getParameter("LateStageTap");
    attachment_mode_switch = std::make_unique<juce::ButtonParameterAttachment>(
        *param, mode_switch);

    const bool isPost = param != nullptr && param->getValue() >= 0.5f;
    mode_switch.setToggleState(isPost, juce::NotificationType::dontSendNotification);
    mode = isPost ? RouteBlock::Mode::POST : RouteBlock::Mode::PRE;

    addAndMakeVisible(&mode_switch);

    setupNumberBoxSlider(lineCount, state.getParameter("LineCount"));
    lineCount_attachment = std::make_unique<juce::SliderParameterAttachment>(
        *state.getParameter("LineCount"), lineCount);

    mode_switch.onStateChange = [this]() {
        const auto newMode = mode_switch.getToggleState() ? RouteBlock::Mode::POST : RouteBlock::Mode::PRE;
        if (newMode != mode)
        {
            previousMode = mode;
            mode = newMode;
            diagramTransition = 0.0f;
            diagramTransitionActive = true;
            startTimerHz(60);
        }
        repaint();
    };
}

void RouteBlock::paint(juce::Graphics &g)
{
    Block::paint(g);
    layout.drawTitle(g, "Route", getLocalBounds());
    layout.drawNumberBoxTitle(g, &lineCount, "PARALLEL LINES", getLocalBounds());
    layout.drawTextRightToSlider(g, &mode_switch, getLocalBounds());

    if (diagramTransitionActive)
    {
        g.beginTransparencyLayer(1.0f - diagramTransition);
        layout.drawCustomDiagram(g, previousMode == RouteBlock::Mode::PRE, getLocalBounds());
        g.endTransparencyLayer();

        g.beginTransparencyLayer(diagramTransition);
        layout.drawCustomDiagram(g, mode == RouteBlock::Mode::PRE, getLocalBounds());
        g.endTransparencyLayer();
    }
    else
    {
        layout.drawCustomDiagram(g, mode == RouteBlock::Mode::PRE, getLocalBounds());
    }
}

void RouteBlock::timerCallback()
{
    diagramTransition += 0.12f;
    if (diagramTransition >= 1.0f)
    {
        diagramTransition = 1.0f;
        diagramTransitionActive = false;
        stopTimer();
    }
    repaint();
}

void RouteBlock::paintOverChildren(juce::Graphics &)
{
}

void RouteBlock::resized()
{
    layout.placeUIs(nullptr, &lineCount, &mode_switch, getLocalBounds());
}

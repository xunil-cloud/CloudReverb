#include "Block.h"
#include "CustomToggleButton.h"
#include "layout/Layout.h"

class RouteBlock : public Block, private juce::Timer
{
public:
    RouteBlock(const juce::String &, const juce::AudioProcessorValueTreeState &);
    void paint(juce::Graphics &g) override;
    void paintOverChildren(juce::Graphics &g) override;
    void resized() override;

    enum class Mode
    {
        PRE = 1,
        POST
    };

private:
    void timerCallback() override;

    CustomToggleButton mode_switch{CustomToggleButton::Style::rectSwitch, "pre/post"};
    std::unique_ptr<juce::ButtonParameterAttachment> attachment_mode_switch;
    juce::Slider lineCount;
    std::unique_ptr<juce::SliderParameterAttachment> lineCount_attachment;
    std::unique_ptr<juce::Drawable> diagram;
    RouteLayout layout;
    Mode mode{Mode::PRE};
    Mode previousMode{Mode::PRE};
    float diagramTransition{1.0f};
    bool diagramTransitionActive{false};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RouteBlock)
};

#include "Block.h"
#include "BinaryData.h"
#include "layout/Layout.h"

class RouteBlock : public Block
{
public:
    RouteBlock(const juce::String &, const juce::AudioProcessorValueTreeState &);
    ~RouteBlock();
    void paint(juce::Graphics &g) override;
    void paintOverChildren(juce::Graphics &g) override;
    void resized() override;

    enum class Mode
    {
        PRE = 1,
        POST
    };

private:
    juce::ToggleButton mode_switch;
    juce::LookAndFeel_V4 defaultLook;
    std::unique_ptr<juce::ButtonParameterAttachment> attachment_mode_switch;
    std::unique_ptr<juce::Drawable> diagram;
    RouteLayout layout;
    Mode mode;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RouteBlock)
};

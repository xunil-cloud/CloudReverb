#include "Block.h"

class RouteBlock : public Block
{
public:
    RouteBlock(const juce::String &, const juce::AudioProcessorValueTreeState &);
    ~RouteBlock();
    // void paint(juce::Graphics&) override;
    void resized() override;

    const std::vector<juce::String> params_id{"InputMix", "PreDelay", "HighPass", "LowPass"};

private:
    juce::ToggleButton mode_switch;
    juce::LookAndFeel_V4 defaultLook;
    std::unique_ptr<juce::ButtonParameterAttachment> attachment_mode_switch;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RouteBlock)
};

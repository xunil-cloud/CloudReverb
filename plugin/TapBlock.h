#include "Block.h"
#include "layout/Layout.h"
#include "CustomToggleButton.h"

class TapBlock : public Block
{
public:
    TapBlock(const juce::String &, const juce::AudioProcessorValueTreeState &);
    void paint(juce::Graphics &g) override;
    void resized() override;

private:
    TapLayout layout;
    CustomToggleButton enableButton{CustomToggleButton::Style::roundedRect, "tap enable"};
    std::unique_ptr<juce::ButtonParameterAttachment> enable_attachment;
    juce::Slider tapCount;
    std::unique_ptr<juce::SliderParameterAttachment> tapCount_attachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TapBlock)
};

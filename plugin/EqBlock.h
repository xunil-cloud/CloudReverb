#include "Block.h"
#include "CustomToggleButton.h"
#include "layout/Layout.h"

class EqBlock : public Block
{
public:
    EqBlock(const juce::String &, const juce::AudioProcessorValueTreeState &);
    void paint(juce::Graphics &) override;
    void resized() override;

private:
    CustomToggleButton lowButton{CustomToggleButton::Style::circle};
    std::unique_ptr<juce::ButtonParameterAttachment> LowButton_attachment;
    CustomToggleButton HighButton{CustomToggleButton::Style::circle};
    std::unique_ptr<juce::ButtonParameterAttachment> HighButton_attachment;
    CustomToggleButton cutoffButton{CustomToggleButton::Style::circle};
    std::unique_ptr<juce::ButtonParameterAttachment> cutoffButton_attachment;
    EqLayout layout;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EqBlock)
};

#include "Block.h"
#include "CustomToggleButton.h"
#include "layout/Layout.h"

class InputBlock : public Block
{
public:
    InputBlock(const juce::String &, const juce::AudioProcessorValueTreeState &);
    void paint(juce::Graphics &g) override;
    void resized() override;

private:
    CustomToggleButton lowButton{CustomToggleButton::Style::circle};
    std::unique_ptr<juce::ButtonParameterAttachment> LowButton_attachment;
    CustomToggleButton HighButton{CustomToggleButton::Style::circle};
    std::unique_ptr<juce::ButtonParameterAttachment> HighButton_attachment;
    InputLayout layout;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InputBlock)
};

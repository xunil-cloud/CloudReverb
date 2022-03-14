#include "Block.h"

class InputBlock : public Block
{
public:
    InputBlock(const juce::String &, const juce::AudioProcessorValueTreeState &);
    void resized() override;

private:
    juce::ToggleButton lowButton;
    std::unique_ptr<juce::ButtonParameterAttachment> LowButton_attachment;
    juce::ToggleButton HighButton;
    std::unique_ptr<juce::ButtonParameterAttachment> HighButton_attachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InputBlock)
};

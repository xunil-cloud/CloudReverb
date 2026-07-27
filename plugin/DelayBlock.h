#include "Block.h"
#include "layout/ThreePartLayout.h"
#include "CustomToggleButton.h"

class DelayBlock : public Block
{
public:
    DelayBlock(const juce::String &, const juce::AudioProcessorValueTreeState &);
    void resized() override;
    void paint(juce::Graphics &) override;

private:
    ThreePartLayout layout;
    CustomToggleButton enableButton{CustomToggleButton::Style::roundedRect, "late delay enable"};
    std::unique_ptr<juce::ButtonParameterAttachment> enable_attachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayBlock)
};

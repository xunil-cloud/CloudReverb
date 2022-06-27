#include "Block.h"
#include "layout/ThreePartLayout.h"

class DelayBlock : public Block
{
public:
    DelayBlock(const juce::String &, const juce::AudioProcessorValueTreeState &);
    void resized() override;
    void paint(juce::Graphics &) override;

private:
    ThreePartLayout layout;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayBlock)
};

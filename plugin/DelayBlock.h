#include "Block.h"

class DelayBlock : public Block
{
public:
    DelayBlock(const juce::String &, const juce::AudioProcessorValueTreeState &);
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayBlock)
};

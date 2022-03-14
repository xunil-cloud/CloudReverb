#include "Block.h"

class TapBlock : public Block
{
public:
    TapBlock(const juce::String &, const juce::AudioProcessorValueTreeState &);
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TapBlock)
};

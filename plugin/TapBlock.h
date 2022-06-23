#include "Block.h"
#include "layout/Layout.h"

class TapBlock : public Block
{
public:
    TapBlock(const juce::String &, const juce::AudioProcessorValueTreeState &);
    void paint(juce::Graphics &g) override;
    void resized() override;

private:
    TapLayout layout;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TapBlock)
};

#include "Block.h"

class DelayBlock : public Block
{
public:
    DelayBlock(const juce::String &, const juce::AudioProcessorValueTreeState &);
    // void paint(juce::Graphics&) override;
    void resized() override;

    const std::vector<juce::String> params_id{"InputMix", "PreDelay", "HighPass", "LowPass"};

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayBlock)
};

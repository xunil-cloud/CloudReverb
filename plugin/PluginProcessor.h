#pragma once

#include <atomic>
#include <cstdint>
#include <array>
#include <juce_audio_processors/juce_audio_processors.h>

#include <unordered_map>

#include "../audio_engine/ReverbController.h"
#include "../concurrentqueue/concurrentqueue.h"
#include "presets/presets.h"

struct ui_state
{
    int width;
    int height;
    int preset_id;
};

class UI_State
{
public:
    void set_state(struct ui_state *state)
    {
        uint64_t current = packed_state.load(std::memory_order_acquire);
        uint64_t new_val;
        do
        {
            int w = (current >> 48) & 0xFFFF;
            int h = (current >> 32) & 0xFFFF;
            int p = current & 0xFFFFFFFF;

            if (state->width && state->height)
            {
                w = state->width;
                h = state->height;
            }
            if (state->preset_id)
            {
                p = state->preset_id;
            }

            new_val = (static_cast<uint64_t>(w) << 48) | (static_cast<uint64_t>(h) << 32) | static_cast<uint64_t>(p);
        } while (!packed_state.compare_exchange_weak(current, new_val, std::memory_order_release, std::memory_order_relaxed));
    }

    struct ui_state get_state()
    {
        uint64_t current = packed_state.load(std::memory_order_acquire);
        struct ui_state state;
        state.width = (current >> 48) & 0xFFFF;
        state.height = (current >> 32) & 0xFFFF;
        state.preset_id = current & 0xFFFFFFFF;
        return state;
    }

private:
    std::atomic<uint64_t> packed_state{1};
};
class AudioPluginAudioProcessorEditor;

//==============================================================================
class AudioPluginAudioProcessor : public juce::AudioProcessor,
                                  private juce::AudioProcessorValueTreeState::Listener,
                                  public juce::AsyncUpdater
{
public:
    //==============================================================================
    AudioPluginAudioProcessor();
    ~AudioPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void parameterChanged(const juce::String &parameterID, float newValue) override;

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;
    void processBlockBypassed(juce::AudioBuffer<float> &buffer,
                              juce::MidiBuffer &midiMessages) override;

    //==============================================================================
    juce::AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String &newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;
    void setPreset(cloudPresets::preset preset);
    void setCurrentPresetDisplayName(const juce::String &name);
    juce::String getCurrentPresetDisplayName() const;
    void handleAsyncUpdate() override;
    void reset() override;
    void requestPresetTransition(int fadeTimeMs = 50);
    juce::UndoManager undoManager{10000, 100};
    juce::AudioProcessorValueTreeState treeState;

    std::unordered_map<juce::String, Parameter> map;
    std::unordered_map<juce::String, juce::RangedAudioParameter *> paramCache;
    struct Message
    {
        float newNormalisedValue;
        float newScaledValue;
        Parameter param;
    };
    UI_State state;

private:
    juce::String currentPresetDisplayName;

    enum class CpuPath
    {
        Scalar,
        SSE2,
        AVX2
    };

    static constexpr int parameterCount = static_cast<int>(Parameter::Count);

    static CpuPath detectCpuPath();
    static const char *getCpuPathName(CpuPath path);

    CloudSeed::ReverbController reverb;
    CpuPath cpuPath{CpuPath::Scalar};
    bool is_bypassed{false};
    std::atomic<bool> presetTransitionRequested{false};
    std::atomic<int> presetTransitionFadeMs{50};
    int presetFadeInSamplesRemaining{0};
    int presetFadeInSamplesTotal{0};
    bool smartIdleActive{false};
    int smartIdleSilentSamples{0};

    std::array<Message, parameterCount> smoothedParams{};
    std::array<Message, parameterCount> targetParams{};
    std::array<int, parameterCount> smoothingSamplesRemaining{};
    std::array<bool, parameterCount> smootherInitialised{};

    moodycamel::ConcurrentQueue<Message> queue;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessor)
};

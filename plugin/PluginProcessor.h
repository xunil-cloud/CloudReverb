#pragma once

#include <atomic>
#include <cstdint>
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
        // sequence lock, increase counter by 1 before and after write
        counter.fetch_add(1, std::memory_order_release);

        // only update state if value is not zero.
        if (state->width && state->height)
        {
            m_state.width = state->width;
            m_state.height = state->height;
        }
        if (state->preset_id)
            m_state.preset_id = state->preset_id;
        counter.fetch_add(1, std::memory_order_release);
    }

    struct ui_state get_state()
    {
        struct ui_state state = {0, 0, 0};
        uint32_t num_before;
        uint32_t num_after;

        // sequence lock
        while (true)
        {

            num_before = counter.load(std::memory_order_acquire);
            if (num_before & 1) // other thread is modifying state, retry
                continue;
            state = m_state;
            num_after = counter.load(std::memory_order_acquire);

            // state is possibly modified by other thread,
            // read data again
            if (num_before != num_after)
            {
                continue;
            }
            break;
        }

        return state;
    }

private:
    std::atomic<uint32_t> counter{0};
    struct ui_state m_state{0, 0, 1};
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
    void setPreset(cloudPresets::preset preset) const;
    void handleAsyncUpdate() override;
    void reset() override;
    juce::AudioProcessorValueTreeState treeState;

    std::unordered_map<juce::String, Parameter> map;
    struct Message
    {
        float newNormalisedValue;
        float newScaledValue;
        Parameter param;
    };
    UI_State state;

private:
    CloudSeed::ReverbController reverb;
    bool is_bypassed{false};

    moodycamel::ConcurrentQueue<Message> queue;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessor)
};

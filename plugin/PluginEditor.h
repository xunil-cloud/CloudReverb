#pragma once

#include "DiffusionBlock.h"
#include "LateDiffusionBlock.h"
#include "InputBlock.h"
#include "ReverbSlider.h"
#include "TapBlock.h"
#include "DelayBlock.h"
#include "Mixer.h"
#include "EqBlock.h"
#include "RouteBlock.h"
#include "Header.h"
#include "PluginProcessor.h"
#include "MyLookAndFeel.h"
#include <vector>

//==============================================================================
class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor,
                                        public juce::ComboBox::Listener,
                                        private juce::Timer
{
public:
    explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent &event) override;
    void mouseUp(const juce::MouseEvent &event) override;
    void paintOverChildren(juce::Graphics &g) override;
    // user has changed the preset
    void comboBoxChanged(juce::ComboBox *comboBox) override;
    void restoreUIstate();

private:
    class TransparentButton : public juce::Button
    {
    public:
        explicit TransparentButton(const juce::String &name) : juce::Button(name) {}
        void paintButton(juce::Graphics &, bool, bool) override {}
    };

    class PresetDropdownComponent;
    class ZoomDropdownComponent;

    void showPresetDropdown();
    void hidePresetDropdown();
    void showZoomDropdown();
    void hideZoomDropdown();
    int findMatchingFactoryPreset() const;
    void updatePresetFieldFromCurrentState();
    void selectFactoryPreset(int presetId);
    void applyFactoryPresetFromHistory(int presetId);
    void recordPresetHistory(int presetId);
    void showFactoryPresetName(int presetId);
    void showCustomPresetName();
    void startThemeTransitionSnapshot();
    void timerCallback() override;
    void applyZoomPercent(int percent);
    int findNearestZoomPercentForCurrentSize() const;
    int calculateCurrentZoomPercent() const;
    void updateZoomDisplayForCurrentSize();

    struct UndoSnapshot
    {
        juce::ValueTree state;
        int presetId{0};
        juce::String displayName{"Custom"};
    };

    void pushUndoSnapshot(int presetId, const juce::String &displayName);
    void applyUndoSnapshot(const UndoSnapshot &snapshot);
    void updateUndoButtons();
    bool isParameterEditComponent(juce::Component *component) const;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor &processorRef;
    MyLookAndFeel myLookAndFeel;
    juce::TextButton reset_button;
    juce::TextButton save_button;
    juce::TextButton load_button;
    juce::TextButton undo_button;
    juce::TextButton redo_button;
    juce::Label undo_label;
    juce::Label redo_label;
    juce::Label zoom_label;
    juce::ComboBox zoom_combo;
    TransparentButton zoom_menu_hitbox{"zoom menu"};
    std::unique_ptr<ZoomDropdownComponent> zoomDropdown;
    juce::DrawableButton theme_switch{"theme", juce::DrawableButton::ImageOnButtonBackground};
    TransparentButton preset_menu_hitbox{"preset menu"};
    std::unique_ptr<PresetDropdownComponent> presetDropdown;
    std::unique_ptr<juce::FileChooser> fileChooser;
    std::unique_ptr<juce::Drawable> sunIcon;
    std::unique_ptr<juce::Drawable> moonIcon;
    bool themeIsLight{false};
    std::vector<int> presetHistory;
    int presetHistoryIndex{-1};
    int lastFactoryPresetId{1};
    int parameterEditDepthSincePreset{0};
    int parameterRedoDepthSincePreset{0};
    juce::ValueTree presetRedoCustomState;
    bool hasPresetRedoCustomState{false};
    bool lastActionWasPreset{false};
    bool customSincePreset{false};
    int baseEditorWidth{0};
    int baseEditorHeight{0};
    bool applyingZoom{false};
    std::vector<UndoSnapshot> undoSnapshots;
    int undoSnapshotIndex{-1};
    bool parameterEditInProgress{false};
    bool applyingUndoSnapshot{false};
    juce::Image themeTransitionSnapshot;
    float themeTransitionAlpha{0.0f};
    uint32_t themeTransitionStartMs{0};

    InputBlock input;
    TapBlock tap;
    DelayBlock delay;
    Mixer mixer;
    EqBlock eq;
    DiffusionBlock diffusion1;
    LateDiffusionBlock diffusion2;
    Header header;
    RouteBlock route;

    std::unique_ptr<juce::ComponentBoundsConstrainer> limit;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};

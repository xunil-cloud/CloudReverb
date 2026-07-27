#ifdef _MSC_VER
#pragma warning(disable: 4702 4244 4305 4458 4100 4996)
#endif

#include "PluginEditor.h"

#include "Block.h"
#include "PluginProcessor.h"
#include "BinaryData.h"
#include <memory>
#include <functional>
#include <cmath>
#include <iterator>
#include <cstdint>
#include <limits>
#include <array>

namespace
{
    constexpr int factoryPresetMinId = 1;
    constexpr int factoryPresetMaxId = 30;

    juce::String toSvgColourString(juce::Colour colour)
    {
        return juce::String::formatted("#%02x%02x%02x",
            (int)colour.getRed(), (int)colour.getGreen(), (int)colour.getBlue());
    }

    bool isTintableSvgPaintValue(const juce::String& value)
    {
        auto v = value.trim().toLowerCase();
        return v.isNotEmpty() && v != "none" && v != "transparent" && !v.startsWith("url(");
    }

    juce::String tintSvgStyleAttribute(const juce::String& style, const juce::String& colourString)
    {
        juce::StringArray declarations;
        declarations.addTokens(style, ";", "");

        juce::String result;
        for (int index = 0; index < declarations.size(); ++index)
        {
            auto declaration = declarations[index].trim();
            if (declaration.isEmpty())
                continue;

            const auto separator = declaration.indexOfChar(':');
            if (separator < 0)
            {
                result << declaration << ";";
                continue;
            }

            const auto property = declaration.substring(0, separator).trim().toLowerCase();
            const auto value = declaration.substring(separator + 1).trim();

            if ((property == "fill" || property == "stroke") && isTintableSvgPaintValue(value))
                result << property << ":" << colourString << ";";
            else
                result << property << ":" << value << ";";
        }

        return result;
    }

    void tintSvgXml(juce::XmlElement& xml, juce::Colour colour, bool isRoot = true)
    {
        const auto colourString = toSvgColourString(colour);

        // Inherit fill/currentColor for SVGs that do not explicitly define a paint colour.
        if (isRoot)
        {
            xml.setAttribute("fill", colourString);
            xml.setAttribute("color", colourString);
        }

        for (auto attribute : { "fill", "stroke" })
        {
            if (xml.hasAttribute(attribute) && isTintableSvgPaintValue(xml.getStringAttribute(attribute)))
                xml.setAttribute(attribute, colourString);
        }

        if (xml.hasAttribute("style"))
            xml.setAttribute("style", tintSvgStyleAttribute(xml.getStringAttribute("style"), colourString));

        for (auto* child = xml.getFirstChildElement(); child != nullptr; child = child->getNextElement())
            tintSvgXml(*child, colour, false);
    }
}

class AudioPluginAudioProcessorEditor::PresetDropdownComponent : public juce::Component
{
public:
    PresetDropdownComponent(juce::StringArray presetNames,
                            int selectedPresetId,
                            int maxHeight,
                            float uiScale,
                            std::function<void()> initCallback,
                            std::function<void()> saveCallback,
                            std::function<void()> loadCallback,
                            std::function<void(int)> presetCallback)
        : names(std::move(presetNames)), selectedId(selectedPresetId), onInit(std::move(initCallback)),
          onSave(std::move(saveCallback)), onLoad(std::move(loadCallback)), onPreset(std::move(presetCallback)),
          list(*this)
    {
        const auto safeScale = juce::jlimit(0.55f, 2.25f, uiScale);
        commandHeight = juce::jmax(18, (int)std::round(28.0f * safeScale));
        itemHeight = juce::jmax(20, (int)std::round(30.0f * safeScale));
        leftTextInset = juce::jmax(18, (int)std::round(28.0f * safeScale));
        fontHeight = juce::jmax(11.0f, 15.0f * safeScale);
        cornerRadius = juce::jmax(2.0f, 3.0f * safeScale);
        borderThickness = juce::jmax(1.0f, 1.5f * safeScale);

        addAndMakeVisible(viewport);
        viewport.setViewedComponent(&list, false);
        viewport.setScrollBarsShown(true, false);

        const auto availableForList = juce::jmax(0, maxHeight - getFixedAreaHeight());
        listHeight = juce::jmin((int)names.size() * itemHeight, availableForList);
        setSize((int)std::round(260.0f * safeScale), getFixedAreaHeight() + listHeight);
    }

    void paint(juce::Graphics &g) override
    {
        const auto bg = MyLookAndFeel::isLightMode ? juce::Colour(0xffdbe3ec) : juce::Colour(0xff0b1017);
        const auto border = MyLookAndFeel::isLightMode ? juce::Colour(0xff8194aa) : juce::Colour(0xff86bde8);
        const auto text = MyLookAndFeel::isLightMode ? juce::Colour(0xff0b1017) : juce::Colour(0xffe0e6ed);
        const auto hover = MyLookAndFeel::isLightMode ? juce::Colour(0xffbfd0e2) : juce::Colour(0xff182334);

        g.setColour(bg);
        g.fillRoundedRectangle(getLocalBounds().toFloat().reduced(0.5f), cornerRadius);
        g.setColour(border);
        g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(0.5f), cornerRadius, borderThickness);

        auto font = MyLookAndFeel::getCustomFont().withHeight(fontHeight);
        g.setFont(font);

        for (int i = 0; i < 3; ++i)
        {
            auto row = getCommandRow(i);
            if (hoveredCommand == i)
            {
                g.setColour(hover);
                g.fillRect(row.reduced(2, 1));
            }

            g.setColour(text);
            const char *label = i == 0 ? "init" : (i == 1 ? "save" : "load");
            g.drawFittedText(label, row.withTrimmedLeft(leftTextInset), juce::Justification::centredLeft, 1);
        }

        g.setColour(border.withAlpha(0.75f));
        g.drawHorizontalLine(commandHeight * 3, 0.0f, (float)getWidth());
    }

    void resized() override
    {
        viewport.setBounds(0, getFixedAreaHeight(), getWidth(), listHeight);
        list.setSize(getWidth() - viewport.getScrollBarThickness(), (int)names.size() * itemHeight);
    }

    void mouseMove(const juce::MouseEvent &event) override
    {
        const auto previous = hoveredCommand;
        hoveredCommand = event.y < commandHeight * 3 ? event.y / commandHeight : -1;
        if (previous != hoveredCommand)
            repaint();
    }

    void mouseExit(const juce::MouseEvent &) override
    {
        hoveredCommand = -1;
        repaint();
    }

    void mouseDown(const juce::MouseEvent &event) override
    {
        if (event.y >= commandHeight * 3)
            return;

        const auto command = event.y / commandHeight;
        if (command == 0 && onInit)
            onInit();
        else if (command == 1 && onSave)
            onSave();
        else if (command == 2 && onLoad)
            onLoad();
    }

private:
    class PresetList : public juce::Component
    {
    public:
        explicit PresetList(PresetDropdownComponent &owner) : dropdown(owner) {}

        void paint(juce::Graphics &g) override
        {
            const auto selected = MyLookAndFeel::isLightMode ? juce::Colour(0xff9db6cf) : juce::Colour(0xff1d2a3d);
            const auto hover = MyLookAndFeel::isLightMode ? juce::Colour(0xffbfd0e2) : juce::Colour(0xff16202e);
            const auto text = MyLookAndFeel::isLightMode ? juce::Colour(0xff0b1017) : juce::Colour(0xffe0e6ed);
            const auto tick = MyLookAndFeel::isLightMode ? juce::Colour(0xff0b1017) : juce::Colour(0xffffffff);

            g.setFont(MyLookAndFeel::getCustomFont().withHeight(dropdown.fontHeight));
            for (int i = 0; i < dropdown.names.size(); ++i)
            {
                auto row = juce::Rectangle<int>(0, i * dropdown.itemHeight, getWidth(), dropdown.itemHeight);
                const auto id = i + factoryPresetFirstId;

                if (id == dropdown.selectedId)
                {
                    g.setColour(selected);
                    g.fillRect(row.reduced(2, 1));

                    auto tickArea = row.removeFromLeft(dropdown.leftTextInset).toFloat();
                    const auto s = dropdown.fontHeight / 15.0f;
                    juce::Path check;
                    check.startNewSubPath(tickArea.getX() + 7.0f * s, tickArea.getCentreY());
                    check.lineTo(tickArea.getX() + 11.0f * s, tickArea.getCentreY() + 4.0f * s);
                    check.lineTo(tickArea.getX() + 19.0f * s, tickArea.getCentreY() - 5.0f * s);
                    g.setColour(tick);
                    g.strokePath(check, juce::PathStrokeType(juce::jmax(1.2f, 1.8f * s), juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
                }
                else if (i == hoveredRow)
                {
                    g.setColour(hover);
                    g.fillRect(row.reduced(2, 1));
                    row.removeFromLeft(dropdown.leftTextInset);
                }
                else
                {
                    row.removeFromLeft(dropdown.leftTextInset);
                }

                g.setColour(text);
                g.drawFittedText(dropdown.names[i], row.withTrimmedRight(6), juce::Justification::centredLeft, 1);
            }
        }

        void mouseMove(const juce::MouseEvent &event) override
        {
            const auto previous = hoveredRow;
            hoveredRow = event.y >= 0 ? event.y / dropdown.itemHeight : -1;
            if (hoveredRow < 0 || hoveredRow >= dropdown.names.size())
                hoveredRow = -1;
            if (previous != hoveredRow)
                repaint();
        }

        void mouseExit(const juce::MouseEvent &) override
        {
            hoveredRow = -1;
            repaint();
        }

        void mouseDown(const juce::MouseEvent &event) override
        {
            const auto row = event.y / dropdown.itemHeight;
            if (row >= 0 && row < dropdown.names.size() && dropdown.onPreset)
                dropdown.onPreset(row + factoryPresetFirstId);
        }

    private:
        PresetDropdownComponent &dropdown;
        int hoveredRow{-1};
    };

    static constexpr int factoryPresetFirstId = 1;

    int commandHeight{28};
    int itemHeight{30};
    int leftTextInset{28};
    float fontHeight{15.0f};
    float cornerRadius{3.0f};
    float borderThickness{1.5f};

    int getFixedAreaHeight() const { return commandHeight * 3 + 1; }
    juce::Rectangle<int> getCommandRow(int index) const { return {0, index * commandHeight, getWidth(), commandHeight}; }

    juce::StringArray names;
    int selectedId{0};
    int listHeight{0};
    int hoveredCommand{-1};
    std::function<void()> onInit;
    std::function<void()> onSave;
    std::function<void()> onLoad;
    std::function<void(int)> onPreset;
    juce::Viewport viewport;
    PresetList list;
};

class AudioPluginAudioProcessorEditor::ZoomDropdownComponent : public juce::Component
{
public:
    ZoomDropdownComponent(int selectedPercent, float uiScale, std::function<void(int)> zoomCallback)
        : selected(selectedPercent), onZoom(std::move(zoomCallback))
    {
        scale = juce::jlimit(0.55f, 2.25f, uiScale);
        rowHeight = juce::jmax(20, (int)std::round(28.0f * scale));
        fontHeight = juce::jmax(11.0f, 15.0f * scale);
        cornerRadius = juce::jmax(2.0f, 3.0f * scale);
        borderThickness = juce::jmax(1.0f, 1.5f * scale);
        setSize((int)std::round(84.0f * scale), (int)values.size() * rowHeight);
    }

    void paint(juce::Graphics &g) override
    {
        const auto bg = MyLookAndFeel::isLightMode ? juce::Colour(0xffdbe3ec) : juce::Colour(0xff0b1017);
        const auto border = MyLookAndFeel::isLightMode ? juce::Colour(0xff8194aa) : juce::Colour(0xff86bde8);
        const auto text = MyLookAndFeel::isLightMode ? juce::Colour(0xff0b1017) : juce::Colour(0xffe0e6ed);
        const auto selectedColour = MyLookAndFeel::isLightMode ? juce::Colour(0xff9db6cf) : juce::Colour(0xff1d2a3d);
        const auto hover = MyLookAndFeel::isLightMode ? juce::Colour(0xffbfd0e2) : juce::Colour(0xff182334);
        const auto tick = MyLookAndFeel::isLightMode ? juce::Colour(0xff0b1017) : juce::Colour(0xffffffff);

        g.setColour(bg);
        g.fillRoundedRectangle(getLocalBounds().toFloat().reduced(0.5f), cornerRadius);
        g.setColour(border);
        g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(0.5f), cornerRadius, borderThickness);

        g.setFont(MyLookAndFeel::getCustomFont().withHeight(fontHeight));

        for (int i = 0; i < (int)values.size(); ++i)
        {
            auto row = juce::Rectangle<int>(0, i * rowHeight, getWidth(), rowHeight);
            if (values[(size_t)i] == selected)
            {
                g.setColour(selectedColour);
                g.fillRect(row.reduced(2, 1));
                auto tickArea = row.removeFromLeft((int)std::round(24.0f * scale)).toFloat();
                juce::Path check;
                check.startNewSubPath(tickArea.getX() + 7.0f * scale, tickArea.getCentreY());
                check.lineTo(tickArea.getX() + 11.0f * scale, tickArea.getCentreY() + 4.0f * scale);
                check.lineTo(tickArea.getX() + 19.0f * scale, tickArea.getCentreY() - 5.0f * scale);
                g.setColour(tick);
                g.strokePath(check, juce::PathStrokeType(juce::jmax(1.2f, 1.8f * scale), juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
            }
            else if (i == hoveredRow)
            {
                g.setColour(hover);
                g.fillRect(row.reduced(2, 1));
                row.removeFromLeft((int)std::round(24.0f * scale));
            }
            else
            {
                row.removeFromLeft((int)std::round(24.0f * scale));
            }

            g.setColour(text);
            g.drawFittedText(juce::String(values[(size_t)i]) + "%", row.withTrimmedRight(6), juce::Justification::centredLeft, 1);
        }
    }

    void mouseMove(const juce::MouseEvent &event) override
    {
        const auto previous = hoveredRow;
        hoveredRow = event.y >= 0 ? event.y / rowHeight : -1;
        if (hoveredRow < 0 || hoveredRow >= (int)values.size())
            hoveredRow = -1;
        if (previous != hoveredRow)
            repaint();
    }

    void mouseExit(const juce::MouseEvent &) override
    {
        hoveredRow = -1;
        repaint();
    }

    void mouseDown(const juce::MouseEvent &event) override
    {
        const auto row = event.y / rowHeight;
        if (row >= 0 && row < (int)values.size() && onZoom)
            onZoom(values[(size_t)row]);
    }

private:
    std::array<int, 9> values{{70, 80, 90, 100, 110, 120, 130, 150, 200}};
    int selected{100};
    int rowHeight{28};
    int hoveredRow{-1};
    float scale{1.0f};
    float fontHeight{15.0f};
    float cornerRadius{3.0f};
    float borderThickness{1.5f};
    std::function<void(int)> onZoom;
};

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p), input("Input", this->processorRef.treeState),
      tap("Multitap Delay", this->processorRef.treeState),
      delay("Late Delay", this->processorRef.treeState),
      mixer("Mixer", this->processorRef.treeState), eq("Shelf EQ", this->processorRef.treeState),
      diffusion1("Early Diffusion", this->processorRef.treeState),
      diffusion2("Late Diffusion", this->processorRef.treeState),
      header("CloudReverb", this->processorRef.treeState),
      route("Route", this->processorRef.treeState)
{
    reset_button.setButtonText("init");
    save_button.setButtonText("Save");
    load_button.setButtonText("Load");
    undo_button.setButtonText(juce::String::fromUTF8("↶"));
    redo_button.setButtonText(juce::String::fromUTF8("↷"));
    undo_button.setTooltip("Undo");
    redo_button.setTooltip("Redo");
    undo_label.setName("UndoRedoCaption");
    redo_label.setName("UndoRedoCaption");
    undo_label.setText("Undo", juce::NotificationType::dontSendNotification);
    redo_label.setText("Redo", juce::NotificationType::dontSendNotification);
    undo_label.setJustificationType(juce::Justification::centred);
    redo_label.setJustificationType(juce::Justification::centred);
    undo_label.setInterceptsMouseClicks(false, false);
    redo_label.setInterceptsMouseClicks(false, false);

    zoom_label.setName("ZoomCaption");
    zoom_label.setText("Zoom", juce::NotificationType::dontSendNotification);
    zoom_label.setJustificationType(juce::Justification::centredLeft);
    zoom_label.setInterceptsMouseClicks(false, false);
    zoom_label.setColour(juce::Label::textColourId, juce::Colour(0xff8a9ba8));
    zoom_combo.addItem("70%", 70);
    zoom_combo.addItem("80%", 80);
    zoom_combo.addItem("90%", 90);
    zoom_combo.addItem("100%", 100);
    zoom_combo.addItem("110%", 110);
    zoom_combo.addItem("120%", 120);
    zoom_combo.addItem("130%", 130);
    zoom_combo.addItem("150%", 150);
    zoom_combo.addItem("200%", 200);
    zoom_combo.setSelectedId(100, juce::NotificationType::dontSendNotification);
    zoom_combo.setInterceptsMouseClicks(false, false);
    zoom_combo.setWantsKeyboardFocus(false);
    zoom_combo.onChange = [this]() {
        if (!applyingZoom)
            applyZoomPercent(zoom_combo.getSelectedId());
    };
    zoom_menu_hitbox.onClick = [this]() {
        showZoomDropdown();
    };

    theme_switch.setColour(juce::DrawableButton::backgroundColourId, juce::Colour(0xff101826));
    theme_switch.setColour(juce::DrawableButton::backgroundOnColourId, juce::Colour(0xff1c2738));

    auto presetDir = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory).getChildFile("CloudReverb").getChildFile("Presets");
    presetDir.createDirectory();

    save_button.onClick = [this, presetDir]() {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Save CloudReverb Preset", presetDir, "*.xml");

        fileChooser->launchAsync(juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser &fc) {
                auto file = fc.getResult();
                if (file != juce::File{})
                {
                    if (auto tree = processorRef.treeState.copyState().createXml())
                    {
                        file.replaceWithText(tree->toString());

                        // After saving a user preset, show its file name in the Presets field.
                        // This is display-only and does not add a permanent item to the factory list.
                        processorRef.setCurrentPresetDisplayName(file.getFileNameWithoutExtension());
                        header.combobox.setText(file.getFileNameWithoutExtension(),
                                                juce::NotificationType::dontSendNotification);
                        pushUndoSnapshot(0, file.getFileNameWithoutExtension());
                    }
                }
            });
    };

    load_button.onClick = [this, presetDir]() {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Load CloudReverb Preset", presetDir, "*.xml");

        fileChooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser &fc) {
                auto file = fc.getResult();
                if (file != juce::File{})
                {
                    if (auto xml = juce::XmlDocument::parse(file))
                    {
                        processorRef.treeState.replaceState(juce::ValueTree::fromXml(*xml));

                        // Show the loaded user preset name in the Presets field without adding
                        // a permanent factory-list item. This is display-only and keeps factory
                        // preset IDs untouched.
                        processorRef.setCurrentPresetDisplayName(file.getFileNameWithoutExtension());
                        header.combobox.setText(file.getFileNameWithoutExtension(),
                                                juce::NotificationType::dontSendNotification);
                        pushUndoSnapshot(0, file.getFileNameWithoutExtension());
                    }
                }
            });
    };

    // Load theme SVG icons from BinaryData (if available)
    {
#if HAS_NIGHT_SVG
        // Night icon: dark asset — shown while LIGHT theme is active
        if (auto moonXml = juce::XmlDocument::parse(juce::String::fromUTF8(BinaryData::Night_svg, BinaryData::Night_svgSize)))
        {
            tintSvgXml(*moonXml, juce::Colour(0xff0d1b2a));
            moonIcon = juce::Drawable::createFromSVG(*moonXml);
        }
#endif

#if HAS_DAY_SVG
        // Day icon: cyan/blue asset — shown while DARK theme is active
        if (auto sunXml = juce::XmlDocument::parse(juce::String::fromUTF8(BinaryData::Day_svg, BinaryData::Day_svgSize)))
        {
            tintSvgXml(*sunXml, juce::Colour(0xff00bcd4));
            sunIcon = juce::Drawable::createFromSVG(*sunXml);
        }
#endif
    }

    // Initial state is Dark theme, so show Day.svg / light icon
    if (sunIcon)
        theme_switch.setImages(sunIcon.get());
    else
    {
        // Fallback: dark theme uses the light/day icon glyph
        theme_switch.setButtonText("☀");
    }

    theme_switch.onClick = [this]() {
        startThemeTransitionSnapshot();
        themeIsLight = !themeIsLight;
        MyLookAndFeel::isLightMode = themeIsLight;
        myLookAndFeel.setLightMode(themeIsLight);
        theme_switch.setColour(juce::DrawableButton::backgroundColourId,
                               themeIsLight ? juce::Colour(0xffd8e0ea) : juce::Colour(0xff101826));
        theme_switch.setColour(juce::DrawableButton::backgroundOnColourId,
                               themeIsLight ? juce::Colour(0xffc3cedc) : juce::Colour(0xff1c2738));

#if HAS_DAY_SVG || HAS_NIGHT_SVG
        // Requested behaviour:
        // Light theme  → Night.svg dark icon
        // Dark theme   → Day.svg light icon
        if (themeIsLight && moonIcon)
            theme_switch.setImages(moonIcon.get());
        else if (!themeIsLight && sunIcon)
            theme_switch.setImages(sunIcon.get());
        else
        {
            theme_switch.setImages(nullptr);
            theme_switch.setButtonText(themeIsLight ? "☾" : "☀");
        }
#else
        // Fallback: toggle text
        theme_switch.setImages(nullptr);
        theme_switch.setButtonText(themeIsLight ? "☾" : "☀");
#endif

        repaint();
        for (auto *child : getChildren())
            child->repaint();
    };

    preset_menu_hitbox.onClick = [this]() {
        showPresetDropdown();
    };

    undo_button.onClick = [this]() {
        if (undoSnapshotIndex > 0)
            applyUndoSnapshot(undoSnapshots[(size_t)--undoSnapshotIndex]);
    };

    redo_button.onClick = [this]() {
        if (undoSnapshotIndex + 1 < (int)undoSnapshots.size())
            applyUndoSnapshot(undoSnapshots[(size_t)++undoSnapshotIndex]);
    };

    reset_button.onClick = [this]() {
        processorRef.requestPresetTransition();
        auto &state = this->processorRef.treeState;
        state.getParameter("InputMix")->setValueNotifyingHost(0.0);
        state.getParameter("PreDelay")->setValueNotifyingHost(0.0);
        state.getParameter("HighPass")->setValueNotifyingHost(0.0);
        state.getParameter("LowPass")->setValueNotifyingHost(0.29000008106231689);
        state.getParameter("TapEnabled")->setValueNotifyingHost(1.0);
        state.getParameter("TapCount")->setValueNotifyingHost(0.35);
        state.getParameter("TapLength")->setValueNotifyingHost(0.5);
        state.getParameter("TapGain")->setValueNotifyingHost(1.0);
        state.getParameter("TapDecay")->setValueNotifyingHost(0.5);
        state.getParameter("DiffusionEnabled")->setValueNotifyingHost(1.0);
        state.getParameter("DiffusionStages")->setValueNotifyingHost(0.4285714328289032);
        state.getParameter("DiffusionDelay")->setValueNotifyingHost(0.5);
        state.getParameter("DiffusionFeedback")->setValueNotifyingHost(0.725000262260437);
        state.getParameter("LateDelayEnabled")->setValueNotifyingHost(1.0);
        state.getParameter("LineCount")->setValueNotifyingHost(1.0);
        state.getParameter("LineDelay")->setValueNotifyingHost(0.5);
        state.getParameter("LineDecay")->setValueNotifyingHost(0.5);
        state.getParameter("LateDiffusionEnabled")->setValueNotifyingHost(1.0);
        state.getParameter("LateDiffusionStages")->setValueNotifyingHost(0.28571429848670959);
        state.getParameter("LateDiffusionDelay")->setValueNotifyingHost(0.5);
        state.getParameter("LateDiffusionFeedback")->setValueNotifyingHost(0.65999996662139893);
        state.getParameter("PostLowShelfGain")->setValueNotifyingHost(0.5199999213218689);
        state.getParameter("PostLowShelfFrequency")->setValueNotifyingHost(0.31499990820884705);
        state.getParameter("PostHighShelfGain")->setValueNotifyingHost(0.83500003814697266);
        state.getParameter("PostHighShelfFrequency")->setValueNotifyingHost(0.73000013828277588);
        state.getParameter("PostCutoffFrequency")->setValueNotifyingHost(0.73499983549118042);
        state.getParameter("EarlyDiffusionModAmount")->setValueNotifyingHost(0.50000005960464478);
        state.getParameter("EarlyDiffusionModRate")->setValueNotifyingHost(0.42500010132789612);
        state.getParameter("LineModAmount")->setValueNotifyingHost(0.59000003337860107);
        state.getParameter("LineModRate")->setValueNotifyingHost(0.46999993920326233);
        state.getParameter("LateDiffusionModAmount")->setValueNotifyingHost(0.619999885559082);
        state.getParameter("LateDiffusionModRate")->setValueNotifyingHost(0.42500019073486328);
        state.getParameter("TapSeed")->setValueNotifyingHost(0.001149);
        state.getParameter("DiffusionSeed")->setValueNotifyingHost(0.000188);
        state.getParameter("DelaySeed")->setValueNotifyingHost(0.000336);
        state.getParameter("PostDiffusionSeed")->setValueNotifyingHost(0.000499);
        state.getParameter("CrossSeed")->setValueNotifyingHost(0.0);
        state.getParameter("DryOut")->setValueNotifyingHost(0.94499987363815308);
        state.getParameter("PredelayOut")->setValueNotifyingHost(0.0);
        state.getParameter("EarlyOut")->setValueNotifyingHost(0.77999997138977051);
        state.getParameter("MainOut")->setValueNotifyingHost(0.74500006437301636);
        if (auto *widthParam = state.getParameter("Width"))
            widthParam->setValueNotifyingHost(widthParam->convertTo0to1(1.0f)); // actual width = 1.0
        state.getParameter("HiPassEnabled")->setValueNotifyingHost(0.0);
        state.getParameter("LowPassEnabled")->setValueNotifyingHost(0.0);
        state.getParameter("LowShelfEnabled")->setValueNotifyingHost(0.0);
        state.getParameter("HighShelfEnabled")->setValueNotifyingHost(0.0);
        state.getParameter("CutoffEnabled")->setValueNotifyingHost(1.0);
        state.getParameter("LateStageTap")->setValueNotifyingHost(1.0);
        state.getParameter("Interpolation")->setValueNotifyingHost(0.);
    };

    struct ui_state state = processorRef.state.get_state();
    header.combobox.setSelectedId(state.preset_id, juce::NotificationType::dontSendNotification);
    if (processorRef.getCurrentPresetDisplayName().isNotEmpty())
        header.combobox.setText(processorRef.getCurrentPresetDisplayName(), juce::NotificationType::dontSendNotification);
    header.combobox.addListener(this);

    // The real ComboBox is used only as a display field. Mouse interaction is handled
    // by preset_menu_hitbox so the custom dropdown is always used and the default
    // JUCE popup can never appear after reopening the editor or changing presets.
    header.combobox.setInterceptsMouseClicks(false, false);
    header.combobox.setWantsKeyboardFocus(false);

    if (state.preset_id >= factoryPresetMinId && state.preset_id <= factoryPresetMaxId)
        recordPresetHistory(state.preset_id);

    const auto *primaryDisplay = juce::Desktop::getInstance().getDisplays().getPrimaryDisplay();

    const auto displayWidth =
        primaryDisplay != nullptr ? primaryDisplay->totalArea.getWidth() : 1920;
    const auto displayHeight =
        primaryDisplay != nullptr ? primaryDisplay->totalArea.getHeight() : 1080;

    const auto userRatio = std::min(displayWidth / 1920.f, displayHeight / 1080.f);
    const auto defaultWidth = 1418.f;
    const auto defaultHeight = 782.f;
    baseEditorWidth = (int)std::round(defaultWidth * 0.9f * userRatio);
    baseEditorHeight = (int)std::round(defaultHeight * 0.9f * userRatio);

    limit = std::make_unique<juce::ComponentBoundsConstrainer>();
    limit->setMinimumSize(defaultWidth * 0.9 * userRatio * 0.4,
                          defaultHeight * 0.9 * userRatio * 0.4);
    limit->setFixedAspectRatio(defaultWidth / defaultHeight);
    setConstrainer(limit.get());
    if (state.width && state.height)
    {
        setSize(state.width, state.height);
        updateZoomDisplayForCurrentSize();
    }
    else
    {
        setSize(baseEditorWidth, baseEditorHeight);
        zoom_combo.setSelectedId(100, juce::NotificationType::dontSendNotification);
    }

    addAndMakeVisible(theme_switch);
    addAndMakeVisible(zoom_label);
    addAndMakeVisible(zoom_combo);
    addAndMakeVisible(zoom_menu_hitbox);
    addAndMakeVisible(undo_button);
    addAndMakeVisible(redo_button);
    addAndMakeVisible(undo_label);
    addAndMakeVisible(redo_label);
    addAndMakeVisible(preset_menu_hitbox);
    addAndMakeVisible(input);
    addAndMakeVisible(tap);
    addAndMakeVisible(delay);
    addAndMakeVisible(diffusion1);
    addAndMakeVisible(diffusion2);
    addAndMakeVisible(mixer);
    addAndMakeVisible(eq);
    addAndMakeVisible(route);
    addAndMakeVisible(header);
    setLookAndFeel(&myLookAndFeel);

    setOpaque(true);
    setResizable(true, true);
    addMouseListener(this, true);
    pushUndoSnapshot(header.combobox.getSelectedId(), header.combobox.getText());
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() { setLookAndFeel(nullptr); }

//==============================================================================
void AudioPluginAudioProcessorEditor::paint(juce::Graphics &g)
{
    bool isLight = MyLookAndFeel::isLightMode;

    if (isLight)
    {
        // Light theme: explicit vertical top-to-bottom Icy background
        juce::Colour bgTop(0xffe2edf7);
        juce::Colour bgBottom(0xfff2f7fc);
        juce::ColourGradient gradient(bgTop, (float)getWidth() * 0.5f, 0.0f,
                                      bgBottom, (float)getWidth() * 0.5f, (float)getHeight(), false);
        g.setGradientFill(gradient);
        g.fillAll();
    }
    else
    {
        // Dark theme: explicit vertical top-to-bottom Navy background
        juce::Colour bgTop(0xff101826);
        juce::Colour bgBottom(0xff0a0f18);
        juce::ColourGradient gradient(bgTop, (float)getWidth() * 0.5f, 0.0f,
                                      bgBottom, (float)getWidth() * 0.5f, (float)getHeight(), false);
        g.setGradientFill(gradient);
        g.fillAll();
    }
}

void AudioPluginAudioProcessorEditor::startThemeTransitionSnapshot()
{
    if (getWidth() <= 0 || getHeight() <= 0)
        return;

    themeTransitionSnapshot = {};
    juce::Image snapshot(juce::Image::ARGB, getWidth(), getHeight(), true);
    juce::Graphics snapshotGraphics(snapshot);
    paintEntireComponent(snapshotGraphics, false);

    themeTransitionSnapshot = snapshot;
    themeTransitionAlpha = 1.0f;
    themeTransitionStartMs = juce::Time::getMillisecondCounter();
    startTimerHz(60);
}

void AudioPluginAudioProcessorEditor::paintOverChildren(juce::Graphics &g)
{
    if (themeTransitionSnapshot.isValid() && themeTransitionAlpha > 0.0f)
    {
        g.setOpacity(themeTransitionAlpha);
        g.drawImageAt(themeTransitionSnapshot, 0, 0);
        g.setOpacity(1.0f);
    }
}

void AudioPluginAudioProcessorEditor::timerCallback()
{
    constexpr float durationMs = 260.0f;
    const auto elapsed = (float)(juce::Time::getMillisecondCounter() - themeTransitionStartMs);
    themeTransitionAlpha = juce::jlimit(0.0f, 1.0f, 1.0f - elapsed / durationMs);

    if (themeTransitionAlpha <= 0.0f)
    {
        themeTransitionSnapshot = {};
        stopTimer();
    }

    repaint();
}

void AudioPluginAudioProcessorEditor::resized()
{
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    using Px = juce::Grid::Px;

    juce::Grid grid;
    grid.templateColumns = {Track(Fr(540)), Track(Fr(540)), Track(Fr(330))};
    grid.templateRows = {Track(Fr(1)), Track(Fr(2)), Track(Fr(2)), Track(Fr(2))};
    const auto zoomScale = baseEditorWidth > 0 ? (float)getWidth() / (float)baseEditorWidth : 1.0f;
    grid.columnGap = Px(4.0f * zoomScale);
    grid.rowGap = Px(4.0f * zoomScale);
    grid.items = {juce::GridItem(header).withArea(1, 1, 1, 4),
                  juce::GridItem(tap),
                  juce::GridItem(diffusion1),
                  juce::GridItem(eq).withArea(2, 3, 4, 3),
                  juce::GridItem(delay),
                  juce::GridItem(diffusion2),
                  juce::GridItem(input),
                  juce::GridItem(route),
                  juce::GridItem(mixer)};
    const auto outerMargin = (int)std::round(12.0f * zoomScale);
    grid.performLayout(getLocalBounds().withTrimmedLeft(outerMargin).withTrimmedRight(outerMargin).withTrimmedBottom(outerMargin));

    const auto topScale = getWidth() / 1418.0f;
    zoom_label.setBounds((int)(232 * topScale), (int)(64 * topScale), (int)(45 * topScale), (int)(25 * topScale));
    zoom_combo.setBounds(zoom_label.getRight() + (int)(2 * topScale), (int)(63 * topScale),
                         (int)(84 * topScale), (int)(26 * topScale));
    zoom_menu_hitbox.setBounds(zoom_combo.getBounds());
    zoom_menu_hitbox.toFront(true);

    if (!applyingZoom)
        updateZoomDisplayForCurrentSize();

    if (zoomDropdown != nullptr)
        showZoomDropdown();

    auto comboBounds = header.combobox.getBounds().translated(header.getX(), header.getY());
    auto comboY = comboBounds.getY();
    auto comboHeight = comboBounds.getHeight();
    const auto buttonGap = juce::jmax(4, (int)(6 * (getWidth() / 1418.f)));
    const auto smallButtonWidth = comboHeight;
    const auto themeSize = comboHeight + 4;
    const auto rightMargin = 30; // more breathing room from the plugin edge

    preset_menu_hitbox.setBounds(comboBounds);
    undo_button.setBounds(comboBounds.getRight() + buttonGap, comboY, smallButtonWidth, comboHeight);
    redo_button.setBounds(undo_button.getRight() + buttonGap, comboY, smallButtonWidth, comboHeight);
    undo_label.setBounds(undo_button.getX() - 6, undo_button.getBottom() + 1, undo_button.getWidth() + 12, 18);
    redo_label.setBounds(redo_button.getX() - 6, redo_button.getBottom() + 1, redo_button.getWidth() + 12, 18);
    theme_switch.setBounds(getWidth() - rightMargin - themeSize, comboY - 2, themeSize, themeSize);
    theme_switch.toFront(false);
    redo_label.toFront(false);
    undo_label.toFront(false);
    redo_button.toFront(false);
    undo_button.toFront(false);
    preset_menu_hitbox.toFront(true); // must stay above Header/ComboBox to prevent default popup

    if (presetDropdown != nullptr)
        showPresetDropdown();

    struct ui_state state
    {
        getWidth(), getHeight(), 0
    };
    processorRef.state.set_state(&state);
}

void AudioPluginAudioProcessorEditor::showZoomDropdown()
{
    const auto scale = baseEditorWidth > 0 ? (float)getWidth() / (float)baseEditorWidth : 1.0f;
    zoomDropdown = std::make_unique<ZoomDropdownComponent>(zoom_combo.getSelectedId(), scale,
        [this](int percent) {
            applyingZoom = true;
            zoom_combo.setSelectedId(percent, juce::NotificationType::dontSendNotification);
            applyingZoom = false;
            hideZoomDropdown();
            applyZoomPercent(percent);
        });

    addAndMakeVisible(zoomDropdown.get());
    const auto comboBounds = zoom_combo.getBounds();
    zoomDropdown->setBounds(comboBounds.getX(), comboBounds.getBottom() + 1,
                            comboBounds.getWidth(), zoomDropdown->getHeight());
    zoomDropdown->toFront(true);
}

void AudioPluginAudioProcessorEditor::hideZoomDropdown()
{
    if (zoomDropdown != nullptr)
    {
        removeChildComponent(zoomDropdown.get());
        zoomDropdown.reset();
    }
}

void AudioPluginAudioProcessorEditor::applyZoomPercent(int percent)
{
    if (percent <= 0 || baseEditorWidth <= 0 || baseEditorHeight <= 0)
        return;

    const auto newWidth = (int)std::round(baseEditorWidth * (percent / 100.0));
    const auto newHeight = (int)std::round(baseEditorHeight * (percent / 100.0));
    setSize(newWidth, newHeight);
}

int AudioPluginAudioProcessorEditor::calculateCurrentZoomPercent() const
{
    if (baseEditorWidth <= 0)
        return 100;

    return juce::jlimit(1, 400, (int)std::round((double)getWidth() / (double)baseEditorWidth * 100.0));
}

int AudioPluginAudioProcessorEditor::findNearestZoomPercentForCurrentSize() const
{
    static constexpr int zoomValues[] = {70, 80, 90, 100, 110, 120, 130, 150, 200};
    const auto currentPercent = calculateCurrentZoomPercent();
    auto best = 100;
    auto bestDistance = std::numeric_limits<int>::max();

    for (auto value : zoomValues)
    {
        const auto distance = std::abs(value - currentPercent);
        if (distance < bestDistance)
        {
            bestDistance = distance;
            best = value;
        }
    }

    return best;
}

void AudioPluginAudioProcessorEditor::updateZoomDisplayForCurrentSize()
{
    const auto currentPercent = calculateCurrentZoomPercent();
    static constexpr int zoomValues[] = {70, 80, 90, 100, 110, 120, 130, 150, 200};

    applyingZoom = true;

    bool matchedPresetValue = false;
    for (auto value : zoomValues)
    {
        if (currentPercent == value)
        {
            zoom_combo.setSelectedId(value, juce::NotificationType::dontSendNotification);
            matchedPresetValue = true;
            break;
        }
    }

    if (!matchedPresetValue)
        zoom_combo.setText(juce::String(currentPercent) + "%", juce::NotificationType::dontSendNotification);

    applyingZoom = false;
}

void AudioPluginAudioProcessorEditor::mouseDown(const juce::MouseEvent &event)
{
    if (presetDropdown != nullptr && !presetDropdown->getBounds().contains(event.getPosition()))
        hidePresetDropdown();
    if (zoomDropdown != nullptr && !zoomDropdown->getBounds().contains(event.getPosition()))
        hideZoomDropdown();

    auto *original = event.originalComponent;
    if (original != &undo_button && original != &redo_button && original != &preset_menu_hitbox
        && original != &theme_switch && isParameterEditComponent(original))
    {
        parameterEditInProgress = true;
    }
}

void AudioPluginAudioProcessorEditor::mouseUp(const juce::MouseEvent &event)
{
    juce::ignoreUnused(event);

    if (parameterEditInProgress && !applyingUndoSnapshot)
    {
        parameterEditInProgress = false;
        pushUndoSnapshot(0, "Custom");
        showCustomPresetName();
    }
}

void AudioPluginAudioProcessorEditor::showPresetDropdown()
{
    juce::StringArray names;
    names.add("Chorus Delay");
    names.add("Dull Echoes");
    names.add("Hyperplane");
    names.add("Medium Space");
    names.add("Noise in the Hallway");
    names.add("Rubi-Ka Fields");
    names.add("Small Room");
    names.add("The 90s Are Back");
    names.add("Through the Looking Glass");
    names.add("Airy Clear Space");
    names.add("Tiny Booth");
    names.add("Small Studio");
    names.add("Warm Room");
    names.add("Bright Room");
    names.add("Short Plate");
    names.add("Drum Chamber");
    names.add("Vocal Chamber");
    names.add("Medium Hall Clear");
    names.add("Wide Hall");
    names.add("Deep Hall");
    names.add("Cathedral Air");
    names.add("Cinematic Bloom");
    names.add("Infinite Chamber");
    names.add("Galactic Bloom");
    names.add("Nebula Drift");
    names.add("Cosmic Shimmer");
    names.add("Frozen Orbit");
    names.add("Black Hole Tail");
    names.add("Starlight Plate");
    names.add("Event Horizon");

    const auto comboBounds = header.combobox.getBounds().translated(header.getX(), header.getY());
    const auto maxHeight = juce::jmax(90, getHeight() - comboBounds.getBottom() - 12);

    presetDropdown = std::make_unique<PresetDropdownComponent>(
        names,
        header.combobox.getSelectedId(),
        maxHeight,
        baseEditorWidth > 0 ? (float)getWidth() / (float)baseEditorWidth : 1.0f,
        [this]() {
            processorRef.undoManager.clearUndoHistory();
            parameterEditDepthSincePreset = 0;
            parameterRedoDepthSincePreset = 0;
            lastActionWasPreset = false;
            customSincePreset = false;
            header.combobox.setSelectedId(Header::commandInitPreset, juce::NotificationType::sendNotification);
            hidePresetDropdown();
        },
        [this]() {
            if (save_button.onClick)
                save_button.onClick();
            hidePresetDropdown();
        },
        [this]() {
            if (load_button.onClick)
                load_button.onClick();
            hidePresetDropdown();
        },
        [this](int presetId) {
            selectFactoryPreset(presetId);
        });

    addAndMakeVisible(presetDropdown.get());
    presetDropdown->setBounds(comboBounds.getX(), comboBounds.getBottom() + 1,
                              comboBounds.getWidth(), presetDropdown->getHeight());
    presetDropdown->toFront(true);
}

void AudioPluginAudioProcessorEditor::hidePresetDropdown()
{
    if (presetDropdown != nullptr)
    {
        removeChildComponent(presetDropdown.get());
        presetDropdown.reset();
    }
}

bool AudioPluginAudioProcessorEditor::isParameterEditComponent(juce::Component *component) const
{
    for (auto *current = component; current != nullptr && current != this;
         current = current->getParentComponent())
    {
        if (dynamic_cast<juce::Slider *>(current) != nullptr
            || dynamic_cast<juce::ToggleButton *>(current) != nullptr)
            return true;
    }

    return false;
}

void AudioPluginAudioProcessorEditor::pushUndoSnapshot(int presetId, const juce::String &displayName)
{
    if (applyingUndoSnapshot)
        return;

    UndoSnapshot snapshot;
    snapshot.state = processorRef.treeState.copyState();
    snapshot.presetId = presetId;
    snapshot.displayName = displayName.isNotEmpty() ? displayName : "Custom";

    if (undoSnapshotIndex >= 0 && undoSnapshotIndex < (int)undoSnapshots.size())
    {
        const auto &current = undoSnapshots[(size_t)undoSnapshotIndex];
        auto currentXml = current.state.createXml();
        auto snapshotXml = snapshot.state.createXml();
        const auto currentString = currentXml != nullptr ? currentXml->toString() : juce::String{};
        const auto snapshotString = snapshotXml != nullptr ? snapshotXml->toString() : juce::String{};
        if (current.displayName == snapshot.displayName
            && current.presetId == snapshot.presetId
            && currentString == snapshotString)
        {
            updateUndoButtons();
            return;
        }
    }

    if (undoSnapshotIndex + 1 < (int)undoSnapshots.size())
        undoSnapshots.erase(undoSnapshots.begin() + undoSnapshotIndex + 1, undoSnapshots.end());

    undoSnapshots.push_back(snapshot);
    if (undoSnapshots.size() > 101)
        undoSnapshots.erase(undoSnapshots.begin());

    undoSnapshotIndex = (int)undoSnapshots.size() - 1;
    updateUndoButtons();
}

void AudioPluginAudioProcessorEditor::applyUndoSnapshot(const UndoSnapshot &snapshot)
{
    applyingUndoSnapshot = true;
    processorRef.requestPresetTransition(20);
    processorRef.treeState.replaceState(snapshot.state.createCopy());

    if (snapshot.presetId >= factoryPresetMinId && snapshot.presetId <= factoryPresetMaxId)
    {
        header.combobox.setSelectedId(snapshot.presetId, juce::NotificationType::dontSendNotification);
        lastFactoryPresetId = snapshot.presetId;
        customSincePreset = false;
        lastActionWasPreset = true;
    }
    else if (snapshot.presetId == Header::commandInitPreset)
    {
        header.combobox.setSelectedId(Header::commandInitPreset, juce::NotificationType::dontSendNotification);
        customSincePreset = false;
        lastActionWasPreset = false;
    }
    else
    {
        header.combobox.setText(snapshot.displayName, juce::NotificationType::dontSendNotification);
        customSincePreset = true;
        lastActionWasPreset = false;
    }

    applyingUndoSnapshot = false;
    updateUndoButtons();
}

void AudioPluginAudioProcessorEditor::updateUndoButtons()
{
    undo_button.setEnabled(undoSnapshotIndex > 0);
    redo_button.setEnabled(undoSnapshotIndex + 1 < (int)undoSnapshots.size());
}

int AudioPluginAudioProcessorEditor::findMatchingFactoryPreset() const
{
    static const char *ids[] = {
        "InputMix", "PreDelay", "HighPass", "LowPass", "TapCount", "TapLength", "TapGain", "TapDecay",
        "DiffusionEnabled", "DiffusionStages", "DiffusionDelay", "DiffusionFeedback", "LineCount", "LineDelay", "LineDecay",
        "LateDiffusionEnabled", "LateDiffusionStages", "LateDiffusionDelay", "LateDiffusionFeedback",
        "PostLowShelfGain", "PostLowShelfFrequency", "PostHighShelfGain", "PostHighShelfFrequency", "PostCutoffFrequency",
        "EarlyDiffusionModAmount", "EarlyDiffusionModRate", "LineModAmount", "LineModRate", "LateDiffusionModAmount", "LateDiffusionModRate",
        "TapSeed", "DiffusionSeed", "DelaySeed", "PostDiffusionSeed", "CrossSeed",
        "DryOut", "PredelayOut", "EarlyOut", "MainOut", "Width",
        "HiPassEnabled", "LowPassEnabled", "LowShelfEnabled", "HighShelfEnabled", "CutoffEnabled", "LateStageTap", "Interpolation"};

    for (int presetIndex = 0; presetIndex < factoryPresetMaxId; ++presetIndex)
    {
        const auto &preset = cloudPresets::presets[presetIndex];
        const double values[] = {
            preset.InputMix, preset.PreDelay, preset.HighPass, preset.LowPass, preset.TapCount, preset.TapLength, preset.TapGain, preset.TapDecay,
            preset.DiffusionEnabled, preset.DiffusionStages, preset.DiffusionDelay, preset.DiffusionFeedback, preset.LineCount, preset.LineDelay, preset.LineDecay,
            preset.LateDiffusionEnabled, preset.LateDiffusionStages, preset.LateDiffusionDelay, preset.LateDiffusionFeedback,
            preset.PostLowShelfGain, preset.PostLowShelfFrequency, preset.PostHighShelfGain, preset.PostHighShelfFrequency, preset.PostCutoffFrequency,
            preset.EarlyDiffusionModAmount, preset.EarlyDiffusionModRate, preset.LineModAmount, preset.LineModRate, preset.LateDiffusionModAmount, preset.LateDiffusionModRate,
            preset.TapSeed, preset.DiffusionSeed, preset.DelaySeed, preset.PostDiffusionSeed, preset.CrossSeed,
            preset.DryOut, preset.PredelayOut, preset.EarlyOut, preset.MainOut, 0.5,
            preset.HiPassEnabled, preset.LowPassEnabled, preset.LowShelfEnabled, preset.HighShelfEnabled, preset.CutoffEnabled, preset.LateStageTap, preset.Interpolation};

        bool matches = true;
        for (int i = 0; i < (int)std::size(ids); ++i)
        {
            auto *parameter = processorRef.treeState.getParameter(ids[i]);
            if (parameter == nullptr || std::abs(parameter->getValue() - (float)values[i]) > 0.0008f)
            {
                matches = false;
                break;
            }
        }

        if (matches)
            return presetIndex + 1;
    }

    return 0;
}

void AudioPluginAudioProcessorEditor::updatePresetFieldFromCurrentState()
{
    const auto presetId = findMatchingFactoryPreset();
    if (presetId >= factoryPresetMinId && presetId <= factoryPresetMaxId)
    {
        header.combobox.setSelectedId(presetId, juce::NotificationType::dontSendNotification);
        struct ui_state state
        {
            0, 0, presetId
        };
        processorRef.state.set_state(&state);
        lastFactoryPresetId = presetId;
        customSincePreset = false;
        lastActionWasPreset = true;
    }
    else
    {
        header.combobox.setText("Custom", juce::NotificationType::dontSendNotification);
        customSincePreset = true;
        lastActionWasPreset = false;
    }
}

void AudioPluginAudioProcessorEditor::showFactoryPresetName(int presetId)
{
    if (presetId >= factoryPresetMinId && presetId <= factoryPresetMaxId)
        header.combobox.setSelectedId(presetId, juce::NotificationType::dontSendNotification);
}

void AudioPluginAudioProcessorEditor::showCustomPresetName()
{
    header.combobox.setText("Custom", juce::NotificationType::dontSendNotification);
}

void AudioPluginAudioProcessorEditor::recordPresetHistory(int presetId)
{
    if (presetId < factoryPresetMinId || presetId > factoryPresetMaxId)
        return;

    if (presetHistoryIndex >= 0 && presetHistoryIndex < (int)presetHistory.size()
        && presetHistory[presetHistoryIndex] == presetId)
    {
        lastFactoryPresetId = presetId;
        parameterEditDepthSincePreset = 0;
        parameterRedoDepthSincePreset = 0;
        lastActionWasPreset = true;
        customSincePreset = false;
        return;
    }

    if (presetHistoryIndex + 1 < (int)presetHistory.size())
        presetHistory.erase(presetHistory.begin() + presetHistoryIndex + 1, presetHistory.end());

    presetHistory.push_back(presetId);
    hasPresetRedoCustomState = false;
    if (presetHistory.size() > 100)
        presetHistory.erase(presetHistory.begin());
    presetHistoryIndex = (int)presetHistory.size() - 1;
    lastFactoryPresetId = presetId;
    parameterEditDepthSincePreset = 0;
    parameterRedoDepthSincePreset = 0;
    lastActionWasPreset = true;
    customSincePreset = false;
}

void AudioPluginAudioProcessorEditor::applyFactoryPresetFromHistory(int presetId)
{
    if (presetId < factoryPresetMinId || presetId > factoryPresetMaxId)
        return;

    showFactoryPresetName(presetId);

    struct ui_state state
    {
        0, 0, presetId
    };
    processorRef.state.set_state(&state);
    processorRef.requestPresetTransition();
    processorRef.setPreset(cloudPresets::presets[presetId - 1]);
    processorRef.undoManager.clearUndoHistory();
    lastFactoryPresetId = presetId;
    parameterEditDepthSincePreset = 0;
    parameterRedoDepthSincePreset = 0;
    lastActionWasPreset = true;
    customSincePreset = false;
}

void AudioPluginAudioProcessorEditor::selectFactoryPreset(int presetId)
{
    if (presetId < factoryPresetMinId || presetId > factoryPresetMaxId)
        return;

    applyFactoryPresetFromHistory(presetId);
    recordPresetHistory(presetId);
    pushUndoSnapshot(presetId, header.combobox.getText());
    hidePresetDropdown();
}

void AudioPluginAudioProcessorEditor::comboBoxChanged(juce::ComboBox *comboBox)
{
    const int i = comboBox->getSelectedId();

    const auto restoreStablePresetSelection = [this]() {
        const auto currentState = processorRef.state.get_state();
        if (currentState.preset_id >= factoryPresetMinId && currentState.preset_id <= factoryPresetMaxId)
            header.combobox.setSelectedId(currentState.preset_id, juce::NotificationType::dontSendNotification);
        else if (currentState.preset_id == Header::commandInitPreset)
            header.combobox.setSelectedId(Header::commandInitPreset, juce::NotificationType::dontSendNotification);
        else
            header.combobox.setSelectedId(0, juce::NotificationType::dontSendNotification);
    };

    if (i == Header::commandInitPreset)
    {
        if (reset_button.onClick)
            reset_button.onClick();

        // Keep "init" visible in the Presets combo after the command is chosen.
        struct ui_state state
        {
            0, 0, Header::commandInitPreset
        };
        processorRef.setCurrentPresetDisplayName("init");
        processorRef.state.set_state(&state);
        lastActionWasPreset = false;
        customSincePreset = false;
        pushUndoSnapshot(Header::commandInitPreset, "init");
        return;
    }

    if (i == Header::commandSavePreset)
    {
        if (save_button.onClick)
            save_button.onClick();

        // Command item: do not leave save selected, so it can be triggered again immediately.
        restoreStablePresetSelection();
        return;
    }

    if (i == Header::commandLoadPreset)
    {
        if (load_button.onClick)
            load_button.onClick();

        // Command item: do not leave load selected, so it can be triggered again immediately.
        restoreStablePresetSelection();
        return;
    }

    if (i < factoryPresetMinId || i > factoryPresetMaxId)
        return;

    applyFactoryPresetFromHistory(i);
    recordPresetHistory(i);
    pushUndoSnapshot(i, header.combobox.getText());
}

void AudioPluginAudioProcessorEditor::restoreUIstate()
{
    struct ui_state state = processorRef.state.get_state();
    header.combobox.setSelectedId(state.preset_id, juce::NotificationType::dontSendNotification);
    if (processorRef.getCurrentPresetDisplayName().isNotEmpty())
        header.combobox.setText(processorRef.getCurrentPresetDisplayName(), juce::NotificationType::dontSendNotification);
    setSize(state.width, state.height);
    DBG("restore ui state from async_updater!");
}

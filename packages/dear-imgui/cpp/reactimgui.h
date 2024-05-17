// Dear ImGui: standalone example application for Emscripten, using GLFW + WebGPU
// (Emscripten is a C++-to-javascript compiler, used to publish executables for the web. See https://emscripten.org/)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include <cstring>
#include <string>
#include <sstream>
#include <emscripten.h>
#include <emscripten/bind.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_wgpu.h"
#include "implot.h"
#include "implot_internal.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#include "shared.h"
#include "implotview.h"

#pragma once

class Widget;

class ReactImgui : public ImPlotView {
    private:
        std::unordered_map<int, std::unique_ptr<Widget>> m_widgets;

        void InitWidget(const json& widgetDef);
        void InitButton(const json& val);
        void InitCheckbox(const json& val);
        void InitSlider(const json& val);
        void InitMultiSlider(const json& val);
        void InitInputText(const json& val);
        void InitBasicCombo(const json& val);
        void SetUpFloatFormatChars();

    public:
        std::unordered_map<int, std::vector<int>> m_hierarchy;

        std::unordered_map<int, std::unique_ptr<char[]>> m_floatFormatChars;

        OnTextChangedCallback m_onInputTextChange;
        OnComboChangedCallback m_onComboChange;
        OnNumericValueChangedCallback m_onNumericValueChange;
        OnMultipleNumericValuesChangedCallback m_onMultiValueChange;
        OnBooleanValueChangedCallback m_onBooleanValueChange;
        OnClickCallback m_onClick;

        ReactImgui(
            const char* newWindowId, 
            const char* newGlWindowTitle
        );

        void RenderWidgetById(int id);

        void SetEventHandlers(
            OnTextChangedCallback onInputTextChangeFn,
            OnComboChangedCallback onComboChangeFn,
            OnNumericValueChangedCallback onNumericValueChangeFn,
            OnMultipleNumericValuesChangedCallback onMultiValueChangeFn,
            OnBooleanValueChangedCallback onBooleanValueChangeFn,
            OnClickCallback onClickFn
        );

        void PrepareForRender();

        void Render(int window_width, int window_height);

        void RenderChildren(int id);

        void RenderWidgets(int id = 0);

        void SetWidget(std::string widgetJsonAsString);

        void PatchWidget(int id, std::string widgetJsonAsString);

        void SetChildren(int id, std::vector<int> childIds);

        void AppendChild(int parentId, int childId);

        std::vector<int> GetChildren(int id);

        json GetAvailableFonts();
};


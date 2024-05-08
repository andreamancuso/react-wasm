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
#include <emscripten/val.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_wgpu.h"
#include "implot.h"
#include "implot_internal.h"
#include <nlohmann/json.hpp>

#include "implotview.h"

using json = nlohmann::json;

#pragma once

class Widget;

class ReactImgui : public ImPlotView {
    typedef std::function<void(const json&)> rendererFunction;

    private:
        std::unordered_map<std::string, std::unique_ptr<Widget>> widgets;

        void RenderWidgets();
        void InitWidget(const json& widgetDef);
        void InitButton(const json& val);
        void InitCheckbox(const json& val);
        void InitSlider(const json& val);
        void InitMultiSlider(const json& val);
        void InitInputText(const json& val);
        void InitBasicCombo(const json& val);
        void SetUpFloatFormatChars();

    public:
        std::unordered_map<int, std::vector<int>> hierarchy;

        std::unordered_map<std::string, rendererFunction> rendererFunctionMap;
        std::unordered_map<int, std::unique_ptr<char[]>> floatFormatChars;

        std::shared_ptr<emscripten::val> onInputTextChange;
        std::unique_ptr<emscripten::val> onComboChange;
        std::unique_ptr<emscripten::val> onNumericValueChange;
        std::unique_ptr<emscripten::val> onMultiValueChange;
        std::unique_ptr<emscripten::val> onBooleanValueChange;
        std::unique_ptr<emscripten::val> onClick;

        template <typename T> 
        inline static emscripten::val ConvertArrayPointerToJsArray(T* arr, int size) {
            const T *end = &arr[size];

            emscripten::val jsArray = emscripten::val::array();

            for (T * curr = arr; curr != end; ++curr) {
                jsArray.call<void>("push", *curr);
            }

            return jsArray;
        }

        ReactImgui(
            emscripten::val onInputTextChangeFn,
            emscripten::val onComboChangeFn,
            emscripten::val onNumericValueChangeFn,
            emscripten::val onMultiValueChangeFn,
            emscripten::val onBooleanValueChangeFn,
            emscripten::val onClickFn,
            const char* newWindowId, 
            const char* newGlWindowTitle
        );

        void RenderWidgetById(int id);

        void PrepareForRender();

        void Render(int window_width, int window_height);

        // todo: maybe we can avoid the JSON parsing and use emscripten::val() instead - though we may want to benchmark the 2 approaches...
        void SetWidget(std::string widgetJsonAsString);

        void SetChildren(int id, emscripten::val childIds);

        json GetAvailableFonts();
};


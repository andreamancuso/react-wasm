// Dear ImGui: standalone example application for Emscripten, using GLFW + WebGPU
// (Emscripten is a C++-to-javascript compiler, used to publish executables for the web. See https://emscripten.org/)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#define IMGUI_USE_WCHAR32

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_wgpu.h"
#include "implot.h"
#include "implot_internal.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>
#endif
#include <nlohmann/json.hpp>

#include "glwasm.cpp"
#include "reactimgui.cpp"

using json = nlohmann::json;

EMSCRIPTEN_DECLARE_VAL_TYPE(OnInputTextChangeType);
EMSCRIPTEN_DECLARE_VAL_TYPE(OnComboChangeType);
EMSCRIPTEN_DECLARE_VAL_TYPE(OnNumbericValueChangeType);
EMSCRIPTEN_DECLARE_VAL_TYPE(OnMultiValueChangeType);
EMSCRIPTEN_DECLARE_VAL_TYPE(OnBooleanValueChangeType);
EMSCRIPTEN_DECLARE_VAL_TYPE(OnClickType);

class WasmRunner {
    protected:
        GLWasm* glWasm;
        ReactImgui* view;

    public:
        WasmRunner(
            emscripten::val onInputTextChange,
            emscripten::val onComboChange,
            emscripten::val onNumericValueChange,
            emscripten::val onMultiValueChange,
            emscripten::val onBooleanValueChange,
            emscripten::val onClick
        ) {
            view = new ReactImgui(
                onInputTextChange, 
                onComboChange, 
                onNumericValueChange, 
                onMultiValueChange, 
                onBooleanValueChange,
                onClick,
                "ReactImgui", 
                "ReactImgui"
            );
            glWasm = &GLWasm::GetInstance(view);
        }

        void run(std::string canvasSelector) {
            glWasm->Init(canvasSelector);
        }

        void exit() {
            emscripten_cancel_main_loop();
            emscripten_force_exit(0);
        }

        void resizeWindow(int width, int height) {
            glWasm->SetWindowSize(width, height);
        }

        void setWidgets(std::string widgetsJson) {
            view->SetWidgets(widgetsJson);
        }

        std::string getAvailableFonts() {
            return view->GetAvailableFonts().dump();
        }

        // void appendChartData(double x, double y)  {
        //     view->AppendData(x, y);
        // }

        // void setAxesDecimalPlaces(int x, int y)  {
        //     view->SetAxesDecimalDigits(x, y);
        // }

        // void resetChartData() {
        //     view->ResetData();
        // }
};

void _exit() {
    emscripten_cancel_main_loop();
    emscripten_force_exit(0);
}

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("exit", &_exit);

    emscripten::class_<WasmRunner>("WasmRunner")
    .constructor<OnInputTextChangeType, OnComboChangeType, OnNumbericValueChangeType, OnMultiValueChangeType, OnBooleanValueChangeType, OnClickType>()
    .function("run", &WasmRunner::run)
    .function("exit", &WasmRunner::exit)
    .function("resizeWindow", &WasmRunner::resizeWindow)
    .function("setWidgets", &WasmRunner::setWidgets)
    .function("getAvailableFonts", &WasmRunner::getAvailableFonts)
    // .function("appendChartData", &WasmRunner::appendChartData)
    // .function("setAxesDecimalPlaces", &WasmRunner::setAxesDecimalPlaces)
    // .function("resetChartData", &WasmRunner::resetChartData)
    ;

    emscripten::register_type<OnInputTextChangeType>("(id: string, value: string) => void");
    emscripten::register_type<OnComboChangeType>("(id: string, value: number) => void");
    emscripten::register_type<OnNumbericValueChangeType>("(id: string, value: number) => void");
    emscripten::register_type<OnMultiValueChangeType>("(id: string, ...args: Primitive[]) => void");
    emscripten::register_type<OnBooleanValueChangeType>("(id: string, value: boolean) => void");
    emscripten::register_type<OnClickType>("(id: string) => void");
}

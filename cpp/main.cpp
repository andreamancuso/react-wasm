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
    emscripten::register_type<OnMultiValueChangeType>("(id: string, values: Primitive[]) => void");
    emscripten::register_type<OnBooleanValueChangeType>("(id: string, value: boolean) => void");
    emscripten::register_type<OnClickType>("(id: string) => void");

    emscripten::enum_<ImGuiWindowFlags_>("ImGuiWindowFlags")
        .value("None", ImGuiWindowFlags_None)
        .value("NoTitleBar", ImGuiWindowFlags_NoTitleBar)
        .value("NoResize", ImGuiWindowFlags_NoResize)
        .value("NoMove", ImGuiWindowFlags_NoMove)
        .value("NoScrollbar", ImGuiWindowFlags_NoScrollbar)
        .value("NoScrollWithMouse", ImGuiWindowFlags_NoScrollWithMouse)
        .value("NoCollapse", ImGuiWindowFlags_NoCollapse)
        .value("AlwaysAutoResize", ImGuiWindowFlags_AlwaysAutoResize)
        .value("NoBackground", ImGuiWindowFlags_NoBackground)
        .value("NoSavedSettings", ImGuiWindowFlags_NoSavedSettings)
        .value("NoMouseInputs", ImGuiWindowFlags_NoMouseInputs)
        .value("MenuBar", ImGuiWindowFlags_MenuBar)
        .value("HorizontalScrollbar", ImGuiWindowFlags_HorizontalScrollbar)
        .value("NoFocusOnAppearing", ImGuiWindowFlags_NoFocusOnAppearing)
        .value("NoBringToFrontOnFocus", ImGuiWindowFlags_NoBringToFrontOnFocus)
        .value("AlwaysVerticalScrollbar", ImGuiWindowFlags_AlwaysVerticalScrollbar)
        .value("AlwaysHorizontalScrollbar", ImGuiWindowFlags_AlwaysHorizontalScrollbar)
        .value("NoNavInputs", ImGuiWindowFlags_NoNavInputs)
        .value("NoNavFocus", ImGuiWindowFlags_NoNavFocus)
        .value("UnsavedDocument", ImGuiWindowFlags_UnsavedDocument)
        .value("NoNav", ImGuiWindowFlags_NoNav)
        .value("NoDecoration", ImGuiWindowFlags_NoDecoration)
        .value("NoInputs", ImGuiWindowFlags_NoInputs)
        // Deprecated
        .value("AlwaysUseWindowPadding", ImGuiWindowFlags_AlwaysUseWindowPadding)
        ;

    emscripten::enum_<ImGuiChildFlags_>("ImGuiChildFlags")
        .value("None", ImGuiChildFlags_None)
        .value("Border", ImGuiChildFlags_Border)
        .value("AlwaysUseWindowPadding", ImGuiChildFlags_AlwaysUseWindowPadding)
        .value("ResizeX", ImGuiChildFlags_ResizeX)
        .value("ResizeY", ImGuiChildFlags_ResizeY)
        .value("AutoResizeX", ImGuiChildFlags_AutoResizeX)
        .value("AutoResizeY", ImGuiChildFlags_AutoResizeY)
        .value("AlwaysAutoResize", ImGuiChildFlags_AlwaysAutoResize)
        .value("FrameStyle", ImGuiChildFlags_FrameStyle)
        ;

    emscripten::enum_<ImGuiInputTextFlags_>("ImGuiInputTextFlags")
        .value("None", ImGuiInputTextFlags_None)
        .value("CharsDecimal", ImGuiInputTextFlags_CharsDecimal)
        .value("CharsHexadecimal", ImGuiInputTextFlags_CharsHexadecimal)
        .value("CharsUppercase", ImGuiInputTextFlags_CharsUppercase)
        .value("CharsNoBlank", ImGuiInputTextFlags_CharsNoBlank)
        .value("AutoSelectAll", ImGuiInputTextFlags_AutoSelectAll)
        .value("EnterReturnsTrue", ImGuiInputTextFlags_EnterReturnsTrue)
        .value("CallbackCompletion", ImGuiInputTextFlags_CallbackCompletion)
        .value("CallbackHistory", ImGuiInputTextFlags_CallbackHistory)
        .value("CallbackAlways", ImGuiInputTextFlags_CallbackAlways)
        .value("CallbackCharFilter", ImGuiInputTextFlags_CallbackCharFilter)
        .value("AllowTabInput", ImGuiInputTextFlags_AllowTabInput)
        .value("CtrlEnterForNewLine", ImGuiInputTextFlags_CtrlEnterForNewLine)
        .value("NoHorizontalScroll", ImGuiInputTextFlags_NoHorizontalScroll)
        .value("AlwaysOverwrite", ImGuiInputTextFlags_AlwaysOverwrite)
        .value("ReadOnly", ImGuiInputTextFlags_ReadOnly)
        .value("Password", ImGuiInputTextFlags_Password)
        .value("NoUndoRedo", ImGuiInputTextFlags_NoUndoRedo)
        .value("CharsScientific", ImGuiInputTextFlags_CharsScientific)
        .value("CallbackResize", ImGuiInputTextFlags_CallbackResize)
        .value("CallbackEdit", ImGuiInputTextFlags_CallbackEdit)
        .value("EscapeClearsAll", ImGuiInputTextFlags_EscapeClearsAll)
        ;

    emscripten::enum_<ImGuiTreeNodeFlags_>("ImGuiTreeNodeFlags")
        .value("None", ImGuiTreeNodeFlags_None)
        .value("Selected", ImGuiTreeNodeFlags_Selected)
        .value("Framed", ImGuiTreeNodeFlags_Framed)
        .value("AllowOverlap", ImGuiTreeNodeFlags_AllowOverlap)
        .value("NoTreePushOnOpen", ImGuiTreeNodeFlags_NoTreePushOnOpen)
        .value("NoAutoOpenOnLog", ImGuiTreeNodeFlags_NoAutoOpenOnLog)
        .value("DefaultOpen", ImGuiTreeNodeFlags_DefaultOpen)
        .value("OpenOnDoubleClick", ImGuiTreeNodeFlags_OpenOnDoubleClick)
        .value("OpenOnArrow", ImGuiTreeNodeFlags_OpenOnArrow)
        .value("Leaf", ImGuiTreeNodeFlags_Leaf)
        .value("Bullet", ImGuiTreeNodeFlags_Bullet)
        .value("FramePadding", ImGuiTreeNodeFlags_FramePadding)
        .value("SpanAvailWidth", ImGuiTreeNodeFlags_SpanAvailWidth)
        .value("SpanFullWidth", ImGuiTreeNodeFlags_SpanFullWidth)
        .value("SpanTextWidth", ImGuiTreeNodeFlags_SpanTextWidth)
        .value("SpanAllColumns", ImGuiTreeNodeFlags_SpanAllColumns)
        .value("NavLeftJumpsBackHere", ImGuiTreeNodeFlags_NavLeftJumpsBackHere)
        // .value("ImGuiTreeNodeFlags_NoScrollOnOpen", ImGuiTreeNodeFlags_NoScrollOnOpen) WIP
        .value("CollapsingHeader", ImGuiTreeNodeFlags_CollapsingHeader)
        // Renamed in 1.89.7
        .value("AllowItemOverlap", ImGuiTreeNodeFlags_AllowItemOverlap)
        ;

    emscripten::enum_<ImGuiPopupFlags_>("ImGuiPopupFlags")
        .value("None", ImGuiPopupFlags_None)
        .value("MouseButtonLeft", ImGuiPopupFlags_MouseButtonLeft)
        .value("MouseButtonRight", ImGuiPopupFlags_MouseButtonRight)
        .value("MouseButtonMiddle", ImGuiPopupFlags_MouseButtonMiddle)
        .value("MouseButtonMask_", ImGuiPopupFlags_MouseButtonMask_)
        .value("MouseButtonDefault_", ImGuiPopupFlags_MouseButtonDefault_)
        .value("NoReopen", ImGuiPopupFlags_NoReopen)
        // .value("NoReopenAlwaysNavInit", ImGuiPopupFlags_NoReopenAlwaysNavInit)
        .value("NoOpenOverExistingPopup", ImGuiPopupFlags_NoOpenOverExistingPopup)
        .value("NoOpenOverItems", ImGuiPopupFlags_NoOpenOverItems)
        .value("AnyPopupId", ImGuiPopupFlags_AnyPopupId)
        .value("AnyPopupLevel", ImGuiPopupFlags_AnyPopupLevel)
        .value("AnyPopup", ImGuiPopupFlags_AnyPopup)
        ;

    emscripten::enum_<ImGuiSelectableFlags_>("ImGuiSelectableFlags")
        .value("None", ImGuiSelectableFlags_None)
        .value("DontClosePopups", ImGuiSelectableFlags_DontClosePopups)
        .value("SpanAllColumns", ImGuiSelectableFlags_SpanAllColumns)
        .value("AllowDoubleClick", ImGuiSelectableFlags_AllowDoubleClick)
        .value("Disabled", ImGuiSelectableFlags_Disabled)
        .value("AllowOverlap", ImGuiSelectableFlags_AllowOverlap)
        // Deprecated
        .value("ImGuiSelectableFlags_AllowItemOverlap", ImGuiSelectableFlags_AllowItemOverlap)
        ;

    emscripten::constant("IMGUI_VERSION", emscripten::val(IMGUI_VERSION));
}

#include <napi.h>
#include <thread>
#include <cstdio>
#include <string>
#include <vector>

#include <set>
#include <optional>
#include "imgui.h"
#include "implot.h"
#include "implot_internal.h"
#include <nlohmann/json.hpp>

#include "color_helpers.h"
#include "reactimgui.h"
#include "implot_renderer.h"

using json = nlohmann::json;

template <typename T>
std::vector<T> JsonToVector(std::string& data) {
    auto parsedData = json::parse(data);
    std::vector<T> vec;
    for (auto& [key, item] : parsedData.items()) {
        vec.push_back(item.template get<T>());
    }
    return vec;
}

template <typename T>
std::set<T> JsonToSet(std::string& data) {
    auto parsedData = json::parse(data);
    std::set<T> set;
    for (auto& [key, item] : parsedData.items()) {
        set.insert(item.template get<T>());
    }
    return set;
}

json IntVectorToJson(const std::vector<int>& data) {
    auto jsonArray = json::array();
    for (auto& item : data) {
        jsonArray.push_back(item);
    }
    return jsonArray;
}

json IntSetToJson(const std::set<int>& data) {
    auto jsonArray = json::array();
    for (auto& item : data) {
        jsonArray.push_back(item);
    }
    return jsonArray;
}

class WasmRunner {
    protected:
        ReactImgui* m_reactImgui{};
        ImGuiRenderer* m_renderer{};

    public:
    WasmRunner() = default;

        static void OnInit() {
            // EM_ASM(
            //     { Module.eventHandlers.onInit(); }
            // );
        }

        static void OnTextChanged(const int id, const std::string& value) {
            // EM_ASM_ARGS(
            //     { Module.eventHandlers.onTextChange($0, UTF8ToString($1)); },
            //     id,
            //     value.c_str()
            // );
        }

        static void OnComboChanged(const int id, const int value) {
            // EM_ASM_ARGS(
            //     { Module.eventHandlers.onComboChange($0, $1); },
            //     id,
            //     value
            // );
        }

        static void OnNumericValueChanged(const int id, const float value) {
            // EM_ASM_ARGS(
            //     { Module.eventHandlers.onNumericValueChange($0, $1); },
            //     id,
            //     value
            // );
        }

        static void OnBooleanValueChanged(const int id, const bool value) {
            // EM_ASM_ARGS(
            //     { Module.eventHandlers.onBooleanValueChange($0, $1); },
            //     id,
            //     value
            // );
        }

        // todo: improve
        static void OnMultipleNumericValuesChanged(const int id, const float* values, const int numValues) {
            if (numValues == 2) {
                // EM_ASM_ARGS(
                //     {
                //         Module.eventHandlers.onMultiValueChange($0, [getValue($1+0, 'float'), getValue($1+4, 'float')]);
                //     },
                //     id,
                //     values
                // );
            } else if (numValues == 3) {
                // EM_ASM_ARGS(
                //     {
                //         Module.eventHandlers.onMultiValueChange($0, [getValue($1+0, 'float'), getValue($1+4, 'float'), getValue($1+8, 'float')]);
                //     },
                //     id,
                //     values
                // );
            } else if (numValues == 4) {
                // EM_ASM_ARGS(
                //     {
                //         Module.eventHandlers.onMultiValueChange($0, [getValue($1+0, 'float'), getValue($1+4, 'float'), getValue($1+8, 'float'), getValue($1+12, 'float')]);
                //     },
                //     id,
                //     values
                // );
            }
        }

        static void OnClick(int const id) {
            // EM_ASM_ARGS(
            //     { Module.eventHandlers.onClick($0); },
            //     id
            // );
        }

    // void run(std::string& canvasSelector, std::string& rawFontDefs, std::optional<std::string>& rawStyleOverridesDefs) {
        void run() {
            std::string rawFontDefs;
            m_reactImgui = new ReactImgui("ReactImgui", std::nullopt);
            m_renderer = new ImPlotRenderer(
                m_reactImgui,
                "ReactImgui",
                "ReactImgui",
                rawFontDefs
            );
            m_reactImgui->SetEventHandlers(
                OnInit,
                OnTextChanged,
                OnComboChanged,
                OnNumericValueChanged,
                OnMultipleNumericValuesChanged,
                OnBooleanValueChanged,
                OnClick);
            m_renderer->Init();
        }

        void exit() const {
            // emscripten_cancel_main_loop();
            // emscripten_force_exit(0);
        }

        void resizeWindow(const int width, const int height) const {
            m_renderer->SetWindowSize(width, height);
        }

        void setElement(std::string& elementJsonAsString) const {
            m_reactImgui->QueueCreateElement(elementJsonAsString);
        }

        void patchElement(const int id, std::string& elementJsonAsString) const {
            m_reactImgui->QueuePatchElement(id, elementJsonAsString);
        }

        void elementInternalOp(const int id, std::string& elementJsonAsString) const {
            m_reactImgui->QueueElementInternalOp(id, elementJsonAsString);
        }

        void setChildren(const int id, const std::vector<int>& childrenIds) const {
            m_reactImgui->QueueSetChildren(id, childrenIds);
        }

        void appendChild(const int parentId, const int childId) const {
            m_reactImgui->QueueAppendChild(parentId, childId);
        }

        [[nodiscard]] std::vector<int> getChildren(const int id) const {
            return m_reactImgui->GetChildren(id);
        }

        [[nodiscard]] std::string getAvailableFonts() const {
            return m_renderer->GetAvailableFonts().dump();
        }

        void appendTextToClippedMultiLineTextRenderer(const int id, const std::string& data) const {
            m_reactImgui->AppendTextToClippedMultiLineTextRenderer(id, data);
        }

        [[nodiscard]] std::string getStyle() const {
            json style;

            style["alpha"] = m_reactImgui->m_appStyle.Alpha;
            style["disabledAlpha"] = m_reactImgui->m_appStyle.DisabledAlpha;
            style["windowPadding"] = { m_reactImgui->m_appStyle.WindowPadding.x, m_reactImgui->m_appStyle.WindowPadding.y };
            style["windowRounding"] = m_reactImgui->m_appStyle.WindowRounding;
            style["windowBorderSize"] = m_reactImgui->m_appStyle.WindowBorderSize;
            style["windowMinSize"] = { m_reactImgui->m_appStyle.WindowMinSize.x, m_reactImgui->m_appStyle.WindowMinSize.y };
            style["windowTitleAlign"] = { m_reactImgui->m_appStyle.WindowTitleAlign.x, m_reactImgui->m_appStyle.WindowTitleAlign.y };
            style["windowMenuButtonPosition"] = m_reactImgui->m_appStyle.WindowMenuButtonPosition;
            style["childRounding"] = m_reactImgui->m_appStyle.ChildRounding;
            style["childBorderSize"] = m_reactImgui->m_appStyle.ChildBorderSize;
            style["popupRounding"] = m_reactImgui->m_appStyle.PopupRounding;
            style["popupBorderSize"] = m_reactImgui->m_appStyle.PopupBorderSize;
            style["framePadding"] = { m_reactImgui->m_appStyle.FramePadding.x, m_reactImgui->m_appStyle.FramePadding.y };
            style["frameRounding"] = m_reactImgui->m_appStyle.FrameRounding;
            style["frameBorderSize"] = m_reactImgui->m_appStyle.FrameBorderSize;
            style["itemSpacing"] = { m_reactImgui->m_appStyle.ItemSpacing.x, m_reactImgui->m_appStyle.ItemSpacing.y };
            style["itemInnerSpacing"] = { m_reactImgui->m_appStyle.ItemInnerSpacing.x, m_reactImgui->m_appStyle.ItemInnerSpacing.y };
            style["cellPadding"] = { m_reactImgui->m_appStyle.CellPadding.x, m_reactImgui->m_appStyle.CellPadding.y };
            style["touchExtraPadding"] = { m_reactImgui->m_appStyle.TouchExtraPadding.x, m_reactImgui->m_appStyle.TouchExtraPadding.y };
            style["indentSpacing"] = m_reactImgui->m_appStyle.IndentSpacing;
            style["columnsMinSpacing"] = m_reactImgui->m_appStyle.ColumnsMinSpacing;
            style["scrollbarSize"] = m_reactImgui->m_appStyle.ScrollbarSize;
            style["scrollbarRounding"] = m_reactImgui->m_appStyle.ScrollbarRounding;
            style["grabMinSize"] = m_reactImgui->m_appStyle.GrabMinSize;
            style["grabRounding"] = m_reactImgui->m_appStyle.GrabRounding;
            style["logSliderDeadzone"] = m_reactImgui->m_appStyle.LogSliderDeadzone;
            style["tabRounding"] = m_reactImgui->m_appStyle.TabRounding;
            style["tabBorderSize"] = m_reactImgui->m_appStyle.TabBorderSize;
            style["tabMinWidthForCloseButton"] = m_reactImgui->m_appStyle.TabMinWidthForCloseButton;
            style["tabBarBorderSize"] = m_reactImgui->m_appStyle.TabBarBorderSize;
            style["tableAngledHeadersAngle"] = m_reactImgui->m_appStyle.TableAngledHeadersAngle;
            style["tableAngledHeadersTextAlign"] = { m_reactImgui->m_appStyle.TableAngledHeadersTextAlign.x, m_reactImgui->m_appStyle.TableAngledHeadersTextAlign.y };
            style["colorButtonPosition"] = m_reactImgui->m_appStyle.ColorButtonPosition;
            style["buttonTextAlign"] = { m_reactImgui->m_appStyle.ButtonTextAlign.x, m_reactImgui->m_appStyle.ButtonTextAlign.y };
            style["selectableTextAlign"] = { m_reactImgui->m_appStyle.SelectableTextAlign.x, m_reactImgui->m_appStyle.SelectableTextAlign.y };
            style["separatorTextPadding"] = { m_reactImgui->m_appStyle.SeparatorTextPadding.x, m_reactImgui->m_appStyle.SeparatorTextPadding.y };
            style["displayWindowPadding"] = { m_reactImgui->m_appStyle.DisplayWindowPadding.x, m_reactImgui->m_appStyle.DisplayWindowPadding.y };
            style["displaySafeAreaPadding"] = { m_reactImgui->m_appStyle.DisplaySafeAreaPadding.x, m_reactImgui->m_appStyle.DisplaySafeAreaPadding.y };
            style["mouseCursorScale"] = m_reactImgui->m_appStyle.MouseCursorScale;
            style["antiAliasedLines"] = m_reactImgui->m_appStyle.AntiAliasedLines;
            style["antiAliasedLinesUseTex"] = m_reactImgui->m_appStyle.AntiAliasedLinesUseTex;
            style["antiAliasedFill"] = m_reactImgui->m_appStyle.AntiAliasedFill;
            style["curveTessellationTol"] = m_reactImgui->m_appStyle.CurveTessellationTol;
            style["circleTessellationMaxError"] = m_reactImgui->m_appStyle.CircleTessellationMaxError;

            style["hoverStationaryDelay"] = m_reactImgui->m_appStyle.HoverStationaryDelay;
            style["hoverDelayShort"] = m_reactImgui->m_appStyle.HoverDelayShort;
            style["hoverDelayNormal"] = m_reactImgui->m_appStyle.HoverDelayNormal;

            style["hoverFlagsForTooltipMouse"] = m_reactImgui->m_appStyle.HoverFlagsForTooltipMouse;
            style["hoverFlagsForTooltipNav"] = m_reactImgui->m_appStyle.HoverFlagsForTooltipNav;

            style["colors"] = json::array();

            for (int i = 0; i < ImGuiCol_COUNT; i++) {
                auto maybeValue = IV4toJsonHEXATuple(m_reactImgui->m_appStyle.Colors[i]);

                if (maybeValue.has_value()) {
                    style["colors"].push_back(maybeValue.value());
                }
            }

            return style.dump();
        }

        void patchStyle(std::string& styleDef) const {
            m_reactImgui->PatchStyle(json::parse(styleDef));
        }

        void setDebug(const bool debug) const {
            m_reactImgui->SetDebug(debug);
        }

        void showDebugWindow() const {
            m_reactImgui->ShowDebugWindow();
        }
};

static std::unique_ptr<WasmRunner> pRunner = std::make_unique<WasmRunner>();

// todo: add validation of arguments
// int main(int argc, char* argv[]) {
//     std::string canvasSelector = argv[1];
//     std::string rawFontDefs = argv[2];
//     std::optional<std::string> rawStyleOverridesDefs;
//
//     if (argc > 2) {
//         // third argument is style overrides
//         rawStyleOverridesDefs.emplace(std::string(argv[3]));
//     }
//
//     pRunner->run();
//
//     return 0;
// }

// void _exit() {
//     pRunner->exit();
// }

void resizeWindow(const int width, const int height) {
    pRunner->resizeWindow(width, height);
}

// emscripten::bind cannot receive `elementJson` by reference
void setElement(std::string elementJson) {
    pRunner->setElement(elementJson);
}

void patchElement(const int id, std::string elementJson) {
    pRunner->patchElement(id, elementJson);
}

// emscripten::bind cannot receive `elementJson` by reference
void elementInternalOp(const int id, std::string elementJson) {
    pRunner->elementInternalOp(id, elementJson);
}

// emscripten::bind cannot receive `childrenIds` by reference
void setChildren(const int id, std::string childrenIds) {
    pRunner->setChildren(id, JsonToVector<int>(childrenIds));
}

void appendChild(const int parentId, const int childId) {
    pRunner->appendChild(parentId, childId);
}

std::string getChildren(const int id) {
    return IntVectorToJson(pRunner->getChildren(id)).dump();
}

// emscripten::bind cannot receive `data` by reference
void appendTextToClippedMultiLineTextRenderer(const int id, std::string data) {
    pRunner->appendTextToClippedMultiLineTextRenderer(id, data);
}

std::string getStyle() {
    return pRunner->getStyle();
}

// emscripten::bind cannot receive `styleDef` by reference
void patchStyle(std::string styleDef) {
    return pRunner->patchStyle(styleDef);
}

void setDebug(const bool debug) {
    return pRunner->setDebug(debug);
}

void showDebugWindow() {
    pRunner->showDebugWindow();
}



int run()
{
    pRunner->run();


    return 0;
}

std::thread uiThread(run);

static Napi::Value Test(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    printf("Starting UI thread\n");

//    uiThread.join();

    return env.Null();
}

static Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports["Test"] = Napi::Function::New(env, Test);

  return exports;
}

NODE_API_MODULE(hello, Init)

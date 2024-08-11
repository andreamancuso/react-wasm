#include <string>
#include <mutex>
#include <rpp/rpp.hpp>
#include "imgui.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#include "yoga/YGEnums.h"
#include "implotview.h"

#pragma once

class Widget;
class MapGenerator;
class Element;
class StyledWidget;
class LayoutNode;
struct WidgetStyle;

enum ElementOp {
    OpCreateElement,
    OpPatchElement,
    OpSetChildren,
    OpAppendChild,
};

struct ElementOpDef {
    ElementOp op;
    json data;
};

class ReactImgui : public ImPlotView {
    private:
        std::unordered_map<int, rpp::subjects::serialized_replay_subject<json>> m_elementInternalOpsSubject;

        rpp::subjects::serialized_replay_subject<ElementOpDef> m_elementOpSubject;

        std::unordered_map<std::string, std::function<std::unique_ptr<Element>(const json&, std::optional<WidgetStyle>, ReactImgui*)>> m_element_init_fn;

        std::unordered_map<int, std::unique_ptr<Element>> m_elements;
        std::mutex m_elements_mutex;

        bool m_debug;

        void CreateElement(const json& elementDef);

        void PatchElement(const json& patchDef);

        void HandleElementInternalOp(const json& opDef);

        void SetChildren(const json& opDef);

        void AppendChild(const json& opDef);
        
        void SetUpFloatFormatChars();

        void SetUpElementCreatorFunctions();
        
    public:
        std::unordered_map<int, std::vector<int>> m_hierarchy;
        std::mutex m_hierarchy_mutex;

        std::unordered_map<int, std::unique_ptr<char[]>> m_floatFormatChars;

        ImGuiStyle m_widgetStyle;

        OnInitCallback m_onInit;
        OnTextChangedCallback m_onInputTextChange;
        OnComboChangedCallback m_onComboChange;
        OnNumericValueChangedCallback m_onNumericValueChange;
        OnMultipleNumericValuesChangedCallback m_onMultiValueChange;
        OnBooleanValueChangedCallback m_onBooleanValueChange;
        OnClickCallback m_onClick;

        ReactImgui(
            const char* newWindowId, 
            const char* newGlWindowTitle, 
            std::string& rawFontDefs,
            std::optional<std::string>& rawStyleOverridesDefs
        );

        void SetDebug(bool debug);

        void ShowDebugWindow();

        void RenderElementById(int id);

        void SetUp(char* pCanvasSelector, WGPUDevice device, GLFWwindow* glfwWindow, WGPUTextureFormat wgpu_preferred_fmt) override;

        void SetEventHandlers(
            OnInitCallback onInitFn,
            OnTextChangedCallback onInputTextChangeFn,
            OnComboChangedCallback onComboChangeFn,
            OnNumericValueChangedCallback onNumericValueChangeFn,
            OnMultipleNumericValuesChangedCallback onMultiValueChangeFn,
            OnBooleanValueChangedCallback onBooleanValueChangeFn,
            OnClickCallback onClickFn
        );

        void PrepareForRender() override;

        void RenderDebugWindow();

        void Render(int window_width, int window_height) override;

        void SetChildrenDisplay(int id, YGDisplay display);

        void RenderChildren(int id);

        void RenderElementTree(int id = 0);

        void RenderElements(int id = 0);

        void QueueCreateElement(std::string& elementJsonAsString);

        void QueuePatchElement(int id, std::string& elementJsonAsString);

        void QueueSetChildren(int id, const std::vector<int>& childIds);

        void QueueAppendChild(int parentId, int childId);

        void QueueElementInternalOp(int id, std::string& widgetOpDef);

        void AppendTextToClippedMultiLineTextRenderer(int id, const std::string& data);

        std::vector<int> GetChildren(int id);

        json GetAvailableFonts();

        template <typename T>
        void ExtractNumberFromStyleDef(const json& styleDef, std::string key, T& value);

        void ExtractBooleanFromStyleDef(const json& styleDef, std::string key, bool& value);

        void ExtractImVec2FromStyleDef(const json& styleDef, std::string key, ImVec2& value);

        void PatchStyle(const json& styleDef);

        StyleVarValueRef GetStyleVar(ImGuiStyleVar key);

        ImVec2 CalcTextSize(const StyledWidget* widget, const char* text, const char* text_end = nullptr, bool hide_text_after_double_hash = false, float wrap_width = -1.0f);

        ImFont* GetWidgetFont(const StyledWidget* widget);

        float GetWidgetFontSize(const StyledWidget* widget);

        float GetTextLineHeight(const StyledWidget* widget);

        float GetTextLineHeightWithSpacing(const StyledWidget* widget);

        float GetFrameHeight(const StyledWidget* widget);

        float GetFrameHeightWithSpacing(const StyledWidget* widget);

        void TakeStyleSnapshot();
};

template <typename T, typename std::enable_if<std::is_base_of<Widget, T>::value, int>::type = 0>
std::unique_ptr<T> makeWidget(const json& val, std::optional<WidgetStyle> maybeStyle, ReactImgui* view);

std::unique_ptr<Element> makeElement(const json& val, ReactImgui* view);

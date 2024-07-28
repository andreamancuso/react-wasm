#include <string>
#include <mutex>
#include <rpp/rpp.hpp>
#include "imgui.h"
#include <nlohmann/json.hpp>

#include "mapgenerator.h"

using json = nlohmann::json;

#include "shared.h"
#include "element.h"
#include "implotview.h"

#pragma once

class Widget;
class StyledWidget;
class LayoutNode;
struct BaseStyle;

class ReactImgui : public ImPlotView {
    private:
        int m_mapGeneratorJobCounter = 0;
        std::unordered_map<int, std::unique_ptr<MapGenerator>> m_mapGeneratorJobs;

        std::unordered_map<int, rpp::subjects::replay_subject<TableData>> m_tableSubjects;
        std::mutex m_tableSubjectsMutex;

        std::unordered_map<std::string, std::function<std::unique_ptr<Element>(const json&, std::optional<BaseStyle>, ReactImgui*)>> m_element_init_fn;

        std::unordered_map<int, std::unique_ptr<Element>> m_elements;
        std::mutex m_elements_mutex;

        void InitElement(const json& elementDef);
        
        void SetUpFloatFormatChars();

        void SetUpElementCreatorFunctions();
        
        void HandleTableData(int id, TableData val);
        
        void HandleBufferedTableData(int id, const std::vector<TableData>& val);

    public:
        std::unordered_map<int, std::vector<int>> m_hierarchy;
        std::mutex m_hierarchy_mutex;

        std::unordered_map<int, std::unique_ptr<char[]>> m_floatFormatChars;

        std::unordered_map<int, std::unique_ptr<Texture>> m_textures;

        ImGuiStyle m_baseStyle;

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

        void RenderElementById(int id);

        void SetUp(char* pCanvasSelector, WGPUDevice device, GLFWwindow* glfwWindow, WGPUTextureFormat wgpu_preferred_fmt) override;

        void SetEventHandlers(
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

        void SetElement(std::string& elementJsonAsString);

        void PatchElement(int id, std::string& elementJsonAsString);

        void SetChildren(int id, const std::vector<int>& childIds);

        void AppendChild(int parentId, int childId);

        void AppendDataToTable(int id, std::string& data);

        void RenderMap(int id, double centerX, double centerY, int zoom);

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
std::unique_ptr<T> makeWidget(const json& val, std::optional<BaseStyle> maybeStyle, ReactImgui* view);

std::unique_ptr<Element> makeElement(const json& val, ReactImgui* view);

#include <cstring>
#include <string>
#include <functional>
#include <sstream>
#include <emscripten/bind.h>
#include <rpp/rpp.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_wgpu.h"
#include "implot.h"
#include "implot_internal.h"
#include <nlohmann/json.hpp>

#include "element/layout_node.h"

#include "shared.h"
#include "reactimgui.h"
#include "implotview.h"
#include "widget/button.h"
#include "widget/checkbox.h"
#include "widget/child.h"
#include "widget/clipped_multi_line_text_renderer.h"
#include "widget/collapsing_header.h"
#include "widget/combo.h"
#include "widget/group.h"
#include "widget/input_text.h"
#include "widget/item_tooltip.h"
#include "widget/map_view.h"
#include "widget/multi_slider.h"
#include "widget/separator.h"
#include "widget/separator_text.h"
#include "widget/slider.h"
#include "widget/widget.h"
#include "widget/styled_widget.h"
#include "widget/table.h"
#include "widget/tabs.h"
#include "widget/text.h"
#include "widget/text_wrap.h"
#include "widget/tree_node.h"
#include "widget/window.h"


using json = nlohmann::json;

template <typename T, typename std::enable_if<std::is_base_of<Widget, T>::value, int>::type>
std::unique_ptr<T> makeWidget(const json& val, std::optional<BaseStyle> maybeStyle, ReactImgui* view) {
    return T::makeWidget(val, maybeStyle, view);
}

std::unique_ptr<Element> makeElement(const json& val, ReactImgui* view) {
    return Element::makeElement(val, view);
}

ReactImgui::ReactImgui(
    const char* newWindowId, 
    const char* newGlWindowTitle, 
    std::string& rawFontDefs,
    std::optional<std::string>& rawStyleOverridesDefs
) : ImPlotView(newWindowId, newGlWindowTitle, rawFontDefs) {
    SetUpElementCreatorFunctions();
    SetUpFloatFormatChars();

    if (rawStyleOverridesDefs.has_value()) {
        m_shouldLoadDefaultStyle = false;
        PatchStyle(json::parse(rawStyleOverridesDefs.value()));
    }

    TakeStyleSnapshot();
}

void ReactImgui::SetUp(char* pCanvasSelector, WGPUDevice device, GLFWwindow* glfwWindow, WGPUTextureFormat wgpu_preferred_fmt) {
    ImGuiView::SetUp(pCanvasSelector, device, glfwWindow, wgpu_preferred_fmt);

    auto handler = [this](const ElementOpDef& elementOpDef) {
        switch(elementOpDef.op) {
            case OpCreateElement: {
                CreateElement(elementOpDef.data);
                break;
            }
            case OpPatchElement: {
                PatchElement(elementOpDef.data);
                break;
            }
            case OpSetChildren: {
                SetChildren(elementOpDef.data);
                break;
            }
            case OpAppendChild: {
                AppendChild(elementOpDef.data);
                break;
            }

            default: break;
        }
    };

    m_elementOpSubject = rpp::subjects::serialized_replay_subject<ElementOpDef>{100};
    m_elementOpSubject.get_observable() | rpp::ops::subscribe(handler);

    m_onInit();
};

void ReactImgui::SetUpElementCreatorFunctions() {
    m_element_init_fn["Group"] = &makeWidget<Group>;
    m_element_init_fn["Child"] = &makeWidget<Child>;
    m_element_init_fn["DIWindow"] = &makeWidget<Window>;
    m_element_init_fn["Separator"] = &makeWidget<Separator>;

    m_element_init_fn["CollapsingHeader"] = &makeWidget<CollapsingHeader>;
    m_element_init_fn["TabBar"] = &makeWidget<TabBar>;
    m_element_init_fn["TabItem"] = &makeWidget<TabItem>;
    m_element_init_fn["TreeNode"] = &makeWidget<TreeNode>;

    m_element_init_fn["Table"] = &makeWidget<Table>;
    m_element_init_fn["ClippedMultiLineTextRenderer"] = &makeWidget<ClippedMultiLineTextRenderer>;
    m_element_init_fn["MapView"] = &makeWidget<MapView>;

    m_element_init_fn["ItemTooltip"] = &makeWidget<ItemTooltip>;

    m_element_init_fn["Combo"] = &makeWidget<Combo>;
    m_element_init_fn["Slider"] = &makeWidget<Slider>;
    m_element_init_fn["InputText"] = &makeWidget<InputText>;
    m_element_init_fn["MultiSlider"] = &makeWidget<MultiSlider>;
    m_element_init_fn["Checkbox"] = &makeWidget<Checkbox>;
    m_element_init_fn["Button"] = &makeWidget<Button>;
    
    m_element_init_fn["SeparatorText"] = &makeWidget<SeparatorText>;
    m_element_init_fn["BulletText"] = &makeWidget<BulletText>;
    m_element_init_fn["UnformattedText"] = &makeWidget<UnformattedText>;
    m_element_init_fn["DisabledText"] = &makeWidget<DisabledText>;
    m_element_init_fn["TextWrap"] = &makeWidget<TextWrap>;
};

void ReactImgui::RenderElementById(const int id) {
    if (m_elements[id]->ShouldRender(this)) {
        m_elements[id]->m_layoutNode->SetDisplay(YGDisplayFlex);

        m_elements[id]->PreRender(this);
        m_elements[id]->Render(this);
        m_elements[id]->PostRender(this);
    } else {
        m_elements[id]->m_layoutNode->SetDisplay(YGDisplayNone);
    }
};

void ReactImgui::RenderElements(const int id) {
    if (m_elements.contains(id)) {
        RenderElementById(id);
    }

    if (!m_elements.contains(id) || m_elements[id]->m_handlesChildrenWithinRenderMethod == false) {
        RenderChildren(id);
    }
};

void ReactImgui::RenderChildren(const int id) {
    if (m_hierarchy.contains(id)) {
        if (!m_hierarchy[id].empty()) {
            for (const auto& childId : m_hierarchy[id]) {
                RenderElements(childId);
            }
        }
    }
};

void ReactImgui::SetChildrenDisplay(const int id, const YGDisplay display) {
    if (m_hierarchy.contains(id)) {
        if (!m_hierarchy[id].empty()) {
            for (const auto& childId : m_hierarchy[id]) {
                if (m_elements.contains(childId)) {
                    m_elements[childId]->m_layoutNode->SetDisplay(display);
                }
            }
        }
    }
};

void ReactImgui::CreateElement(const json& elementDef) {
    if (elementDef.is_object() && elementDef.contains("type")) {
        std::string type = elementDef["type"].template get<std::string>();

        if (m_element_init_fn.contains(type) || type == "Node") {
            int id = elementDef["id"].template get<int>();

            const std::lock_guard<std::mutex> elementLock(m_elements_mutex);
            const std::lock_guard<std::mutex> hierarchyLock(m_hierarchy_mutex);

            if (type == "Node") {
                m_elements[id] = makeElement(elementDef, this);
            } else if (m_element_init_fn.contains(type)) {
                m_elements[id] = m_element_init_fn[type](elementDef, StyledWidget::ExtractStyle(elementDef, this), this);
            }

            if (m_elements[id]->HasInternalOps()) {
                m_elementInternalOpsSubject[id] = rpp::subjects::serialized_replay_subject<json>{10};
                auto handler = [this, id](const json& opDef) {
                    m_elements[id]->HandleInternalOp(opDef);
                };
                m_elementInternalOpsSubject[id].get_observable() | rpp::ops::subscribe(handler);
            }

            m_elements[id]->Init();
            
            m_hierarchy[id] = std::vector<int>();

            if (elementDef.is_object() && elementDef.contains("style") && elementDef["style"].is_object()) {
                m_elements[id]->m_layoutNode->ApplyStyle(elementDef["style"]);
            }
        } else {
            printf("unrecognised element type: '%s'\n", type.c_str());
        }
    } else {
        printf("received JSON either not an object or does not contain type property\n");
    }
};

void ReactImgui::SetEventHandlers(
    const OnInitCallback onInitFn,
    const OnTextChangedCallback onInputTextChangeFn,
    const OnComboChangedCallback onComboChangeFn,
    const OnNumericValueChangedCallback onNumericValueChangeFn,
    const OnMultipleNumericValuesChangedCallback onMultiValueChangeFn,
    const OnBooleanValueChangedCallback onBooleanValueChangeFn,
    const OnClickCallback onClickFn
) {
    m_onInit = onInitFn;
    m_onInputTextChange = onInputTextChangeFn;
    m_onComboChange = onComboChangeFn;
    m_onNumericValueChange = onNumericValueChangeFn;
    m_onMultiValueChange = onMultiValueChangeFn;
    m_onBooleanValueChange = onBooleanValueChangeFn;
    m_onClick = onClickFn;

    Widget::onInputTextChange_ = onInputTextChangeFn;
};

void ReactImgui::SetUpFloatFormatChars() {
    m_floatFormatChars[0] = std::make_unique<char[]>(4);
    m_floatFormatChars[1] = std::make_unique<char[]>(4);
    m_floatFormatChars[2] = std::make_unique<char[]>(4);
    m_floatFormatChars[3] = std::make_unique<char[]>(4);
    m_floatFormatChars[4] = std::make_unique<char[]>(4);
    m_floatFormatChars[5] = std::make_unique<char[]>(4);
    m_floatFormatChars[6] = std::make_unique<char[]>(4);
    m_floatFormatChars[7] = std::make_unique<char[]>(4);
    m_floatFormatChars[8] = std::make_unique<char[]>(4);
    m_floatFormatChars[9] = std::make_unique<char[]>(4);

    strcpy(m_floatFormatChars[0].get(), "%.0f");
    strcpy(m_floatFormatChars[1].get(), "%.1f");
    strcpy(m_floatFormatChars[2].get(), "%.2f");
    strcpy(m_floatFormatChars[3].get(), "%.3f");
    strcpy(m_floatFormatChars[4].get(), "%.4f");
    strcpy(m_floatFormatChars[5].get(), "%.5f");
    strcpy(m_floatFormatChars[6].get(), "%.6f");
    strcpy(m_floatFormatChars[7].get(), "%.7f");
    strcpy(m_floatFormatChars[8].get(), "%.8f");
    strcpy(m_floatFormatChars[9].get(), "%.9f");
};

void ReactImgui::PrepareForRender() {
    SetCurrentContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = nullptr;

    if (m_shouldLoadDefaultStyle) {
        ImGui::StyleColorsLight();
    }
};

void ReactImgui::RenderElementTree(const int id) {
    if (m_elements.contains(id)) {
        // float left = YGNodeLayoutGetLeft(m_elements[id]->m_layoutNode->m_node);
        // float top = YGNodeLayoutGetTop(m_elements[id]->m_layoutNode->m_node);
        const float width = YGNodeLayoutGetWidth(m_elements[id]->m_layoutNode->m_node);
        // float height = YGNodeLayoutGetHeight(m_elements[id]->m_layoutNode->m_node);

        if (!YGFloatIsUndefined(width)) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            ImGui::Text("%d", id);

            ImGui::TableNextColumn();

            ImGui::Text("%zu", m_elements[id]->m_layoutNode->GetChildCount());

            ImGui::TableNextColumn();

            ImGui::Text("l: %f t: %f w: %f h: %f", 
                YGNodeLayoutGetLeft(m_elements[id]->m_layoutNode->m_node), 
                YGNodeLayoutGetTop(m_elements[id]->m_layoutNode->m_node), 
                YGNodeLayoutGetWidth(m_elements[id]->m_layoutNode->m_node), 
                YGNodeLayoutGetHeight(m_elements[id]->m_layoutNode->m_node)
            );
        }
    }

    if (m_hierarchy.contains(id)) {
        for (const auto& childId : m_hierarchy[id]) {
            RenderElementTree(childId);
        }
    }
};

void ReactImgui::Render(const int window_width, const int window_height) {
    SetCurrentContext();

    ImGui_ImplWGPU_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    const std::lock_guard<std::mutex> elementsLock(m_elements_mutex);
    const std::lock_guard<std::mutex> hierarchyLock(m_hierarchy_mutex);

    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(window_width, window_height));

    ImGui::Begin(m_windowId, nullptr, m_window_flags);

    RenderElements();

    // *** DEBUG ***
    // RenderDebugWindow();
    // *** END DEBUG ***

    ImGui::End();
    ImGui::Render();
};

void ReactImgui::RenderDebugWindow() {
    ImGui::SetNextWindowSize(ImVec2(800, 700));
    ImGui::Begin("element-tree", NULL);

    if (ImGui::BeginTable("Elements", 3, ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody)) {
        ImGui::TableSetupColumn("Widget ID", ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Child Count", ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Properties", ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_WidthStretch);

        ImGui::TableHeadersRow();

        RenderElementTree();

        ImGui::EndTable();
    }
    
    ImGui::End();
}

template <typename T>
void ReactImgui::ExtractNumberFromStyleDef(const json& styleDef, const std::string key, T& value) {
    if (styleDef.contains(key) && styleDef[key].is_number_unsigned()) {
        value = styleDef[key].template get<T>();
    }
};

void ReactImgui::ExtractBooleanFromStyleDef(const json& styleDef, const std::string key, bool& value) {
    if (styleDef.contains(key) && styleDef[key].is_number_unsigned()) {
        value = styleDef[key].template get<bool>();
    }
};

void ReactImgui::ExtractImVec2FromStyleDef(const json& styleDef, const std::string key, ImVec2& value) {
    if (styleDef.contains(key)  && styleDef[key].is_array() && styleDef[key].size() == 2) {
        value.x = styleDef[key][0].template get<float>();
        value.y = styleDef[key][1].template get<float>();
    }
};

void ReactImgui::PatchStyle(const json& styleDef) {
    if (styleDef.is_object()) {
        ImGuiStyle* style = &GetStyle();

        ExtractNumberFromStyleDef<float>(styleDef, "alpha", style->Alpha);
        ExtractNumberFromStyleDef<float>(styleDef, "disabledAlpha", style->DisabledAlpha);
        ExtractImVec2FromStyleDef(styleDef, "windowPadding", style->WindowPadding);
        ExtractNumberFromStyleDef<float>(styleDef, "windowRounding", style->WindowRounding);
        ExtractNumberFromStyleDef<float>(styleDef, "windowBorderSize", style->WindowBorderSize);
        ExtractImVec2FromStyleDef(styleDef, "windowMinSize", style->WindowMinSize);
        ExtractImVec2FromStyleDef(styleDef, "windowTitleAlign", style->WindowTitleAlign);
        ExtractNumberFromStyleDef<int>(styleDef, "windowMenuButtonPosition", style->WindowMenuButtonPosition);
        ExtractNumberFromStyleDef<float>(styleDef, "childRounding", style->ChildRounding);
        ExtractNumberFromStyleDef<float>(styleDef, "childBorderSize", style->ChildBorderSize);
        ExtractNumberFromStyleDef<float>(styleDef, "popupRounding", style->PopupRounding);
        ExtractNumberFromStyleDef<float>(styleDef, "popupBorderSize", style->PopupBorderSize);
        ExtractImVec2FromStyleDef(styleDef, "framePadding", style->FramePadding);
        ExtractNumberFromStyleDef<float>(styleDef, "frameRounding", style->FrameRounding);
        ExtractNumberFromStyleDef<float>(styleDef, "frameBorderSize", style->FrameBorderSize);
        ExtractImVec2FromStyleDef(styleDef, "itemSpacing", style->ItemSpacing);
        ExtractImVec2FromStyleDef(styleDef, "itemInnerSpacing", style->ItemInnerSpacing);
        ExtractImVec2FromStyleDef(styleDef, "cellPadding", style->CellPadding);
        ExtractImVec2FromStyleDef(styleDef, "touchExtraPadding", style->TouchExtraPadding);
        ExtractNumberFromStyleDef<float>(styleDef, "indentSpacing", style->IndentSpacing);
        ExtractNumberFromStyleDef<float>(styleDef, "columnsMinSpacing", style->ColumnsMinSpacing);
        ExtractNumberFromStyleDef<float>(styleDef, "scrollbarSize", style->ScrollbarSize);
        ExtractNumberFromStyleDef<float>(styleDef, "scrollbarRounding", style->ScrollbarRounding);
        ExtractNumberFromStyleDef<float>(styleDef, "grabMinSize", style->GrabMinSize);
        ExtractNumberFromStyleDef<float>(styleDef, "grabRounding", style->GrabRounding);
        ExtractNumberFromStyleDef<float>(styleDef, "logSliderDeadzone", style->LogSliderDeadzone);
        ExtractNumberFromStyleDef<float>(styleDef, "tabRounding", style->TabRounding);
        ExtractNumberFromStyleDef<float>(styleDef, "tabBorderSize", style->TabBorderSize);
        ExtractNumberFromStyleDef<float>(styleDef, "tabMinWidthForCloseButton", style->TabMinWidthForCloseButton);
        ExtractNumberFromStyleDef<float>(styleDef, "tabBarBorderSize", style->TabBarBorderSize);
        ExtractNumberFromStyleDef<float>(styleDef, "tableAngledHeadersAngle", style->TableAngledHeadersAngle);
        ExtractImVec2FromStyleDef(styleDef, "tableAngledHeadersTextAlign", style->TableAngledHeadersTextAlign);
        ExtractNumberFromStyleDef<int>(styleDef, "colorButtonPosition", style->ColorButtonPosition);
        ExtractImVec2FromStyleDef(styleDef, "buttonTextAlign", style->ButtonTextAlign);
        ExtractImVec2FromStyleDef(styleDef, "selectableTextAlign", style->SelectableTextAlign);
        ExtractNumberFromStyleDef<float>(styleDef, "separatorTextBorderSize", style->SeparatorTextBorderSize);
        ExtractImVec2FromStyleDef(styleDef, "separatorTextAlign", style->SeparatorTextAlign);
        ExtractImVec2FromStyleDef(styleDef, "separatorTextPadding", style->SeparatorTextPadding);
        ExtractImVec2FromStyleDef(styleDef, "displayWindowPadding", style->DisplayWindowPadding);
        ExtractImVec2FromStyleDef(styleDef, "displaySafeAreaPadding", style->DisplaySafeAreaPadding);
        ExtractNumberFromStyleDef<float>(styleDef, "mouseCursorScale", style->MouseCursorScale);
        ExtractBooleanFromStyleDef(styleDef, "antiAliasedLines", style->AntiAliasedLines);
        ExtractBooleanFromStyleDef(styleDef, "antiAliasedLinesUseTex", style->AntiAliasedLinesUseTex);
        ExtractBooleanFromStyleDef(styleDef, "antiAliasedFill", style->AntiAliasedFill);
        ExtractNumberFromStyleDef<float>(styleDef, "curveTessellationTol", style->CurveTessellationTol);
        ExtractNumberFromStyleDef<float>(styleDef, "circleTessellationMaxError", style->CircleTessellationMaxError);
        ExtractNumberFromStyleDef<float>(styleDef, "hoverStationaryDelay", style->HoverStationaryDelay);
        ExtractNumberFromStyleDef<float>(styleDef, "hoverDelayShort", style->HoverDelayShort);
        ExtractNumberFromStyleDef<float>(styleDef, "hoverDelayNormal", style->HoverDelayNormal);
        ExtractNumberFromStyleDef<int>(styleDef, "hoverFlagsForTooltipMouse", style->HoverFlagsForTooltipMouse);
        ExtractNumberFromStyleDef<int>(styleDef, "hoverFlagsForTooltipNav", style->HoverFlagsForTooltipNav);

        if (styleDef.contains("colors") && styleDef["colors"].is_object()) {
            ImVec4* colors = style->Colors;

            for (auto& [colorItemKey, colorItemValue] : styleDef["colors"].items()) {
                auto colorItemKeyAsNumber = stoi(colorItemKey);

                if (colorItemKeyAsNumber >= 0 && colorItemKeyAsNumber < ImGuiCol_COUNT 
                    && colorItemValue.is_array() && colorItemValue.size() == 2) {
                    colors[colorItemKeyAsNumber] = HEXAtoIV4(
                        colorItemValue[0].template get<std::string>().c_str(),
                        colorItemValue[1].template get<float>()
                    );
                }
            }
        }
    }
};

void ReactImgui::TakeStyleSnapshot() {
    const auto style = GetStyle();

    // This is necessary as the style is repeatedly modified during render via push and pop calls
    memcpy(&m_baseStyle, &style, sizeof(style));
};

void ReactImgui::QueueCreateElement(std::string& elementJsonAsString) {
    try {
        ElementOpDef elementOp{OpCreateElement,json::parse(elementJsonAsString)};
        m_elementOpSubject.get_observer().on_next(elementOp);
    } catch (nlohmann::detail::parse_error& parseError) {
        printf("ReactImgui::QueueCreateElement, parse error: %s\n", parseError.what());
    }
};

void ReactImgui::QueuePatchElement(const int id, std::string& elementJsonAsString) {
    try {
        json opDef = json::parse(elementJsonAsString);
        opDef["id"] = id;
        ElementOpDef elementOp{OpPatchElement,opDef};
        m_elementOpSubject.get_observer().on_next(elementOp);
    } catch (nlohmann::detail::parse_error& parseError) {
        printf("ReactImgui::QueuePatchElement, parse error: %s\n", parseError.what());
    }
};

void ReactImgui::QueueAppendChild(int parentId, int childId) {
    try {
        json opDef;
        opDef["parentId"] = parentId;
        opDef["childId"] = childId;
        ElementOpDef elementOp{OpAppendChild,opDef};
        m_elementOpSubject.get_observer().on_next(elementOp);
    } catch (nlohmann::detail::parse_error& parseError) {
        printf("ReactImgui::QueueAppendChild, parse error: %s\n", parseError.what());
    }
};

void ReactImgui::QueueSetChildren(const int parentId, const std::vector<int>& childrenIds) {
    try {
        json opDef;
        opDef["parentId"] = parentId;
        opDef["childrenIds"] = childrenIds;
        ElementOpDef elementOp{OpSetChildren,opDef};
        m_elementOpSubject.get_observer().on_next(elementOp);
    } catch (nlohmann::detail::parse_error& parseError) {
        printf("ReactImgui::QueueSetChildren, parse error: %s\n", parseError.what());
    }
};

void ReactImgui::QueueElementInternalOp(const int id, std::string& widgetOpDef) {
    try {
        const json opDef = json::parse(widgetOpDef);

        if (m_elementInternalOpsSubject.contains(id)) {
            m_elementInternalOpsSubject[id].get_observer().on_next(opDef);
        }
    } catch (nlohmann::detail::parse_error& parseError) {
        printf("ReactImgui::QueueElementInternalOp, parse error: %s\n", parseError.what());
    }
};

void ReactImgui::PatchElement(const json& opDef) {
    auto id = opDef["id"].template get<int>();

    const std::lock_guard<std::mutex> lock(m_elements_mutex);

    if (m_elements.contains(id)) {
        auto pElement = m_elements[id].get();

        pElement->Patch(opDef, this);
    }
}

void ReactImgui::HandleElementInternalOp(const json& opDef) {
    auto id = opDef["id"].template get<int>();

    const std::lock_guard<std::mutex> lock(m_elements_mutex);

    if (m_elements.contains(id)) {
        auto pElement = m_elements[id].get();

        pElement->HandleInternalOp(opDef);
    }
}

void ReactImgui::SetChildren(const json& opDef) {
    const std::lock_guard<std::mutex> elementsLock(m_hierarchy_mutex);
    const std::lock_guard<std::mutex> hierarchyLock(m_elements_mutex);

    auto parentId = opDef["parentId"].template get<int>();
    auto childrenIds = opDef["childrenIds"].template get<std::vector<int>>();

    if (m_elements.contains(parentId)) {
        YGNodeRemoveAllChildren(m_elements[parentId]->m_layoutNode->m_node);

        const auto size = childrenIds.size();

        for (int i = 0; i < size; i++) {
            const auto childId = childrenIds[i];

            if (m_elements.contains(childId)) {
                m_elements[parentId]->m_layoutNode->InsertChild(m_elements[childId]->m_layoutNode.get(), i);
            }
        }
    }

    m_hierarchy[parentId] = childrenIds;
}

void ReactImgui::AppendChild(const json& opDef) {
    auto parentId = opDef["parentId"].template get<int>();
    auto childId = opDef["childId"].template get<int>();

    const std::lock_guard<std::mutex> lock(m_hierarchy_mutex);
    
    if (m_hierarchy.contains(parentId)) {
        if ( std::find(m_hierarchy[parentId].begin(), m_hierarchy[parentId].end(), childId) == m_hierarchy[parentId].end() ) {
            const std::lock_guard<std::mutex> elementsLock(m_elements_mutex);
    
            if (m_elements.contains(childId)) {
                if (!m_elements[childId]->m_isRoot) {
                    auto parentNode = YGNodeGetParent(m_elements[childId]->m_layoutNode->m_node);
    
                    if (!parentNode) {
                        const auto childCount = m_elements[parentId]->m_layoutNode->GetChildCount();
    
                        m_elements[parentId]->m_layoutNode->InsertChild(m_elements[childId]->m_layoutNode.get(), childCount);
                    }
                }
    
                m_hierarchy[parentId].push_back(childId);
            }
        }
    }
}

std::vector<int> ReactImgui::GetChildren(int id) {
    return m_hierarchy[id];
};

json ReactImgui::GetAvailableFonts() {
    SetCurrentContext();
    ImGuiIO& io = ImGui::GetIO();
    json fonts = json::array();

    for (ImFont* font : io.Fonts->Fonts) {
        fonts.push_back(font->GetDebugName());

        printf("%s\n", font->GetDebugName());
    }

    return fonts;
};

// todo: switch to ReactivePlusPlus's BehaviorSubject
void ReactImgui::AppendTextToClippedMultiLineTextRenderer(const int id, const std::string& data) {
    const std::lock_guard<std::mutex> lock(m_elements_mutex);

    if (m_elements.contains(id) && std::strcmp(m_elements[id]->GetElementType(), "widget") != 0) {
        const auto element = dynamic_cast<Widget*>(m_elements[id].get());

        if (element->m_type == "ClippedMultiLineTextRenderer") {
            dynamic_cast<ClippedMultiLineTextRenderer*>(m_elements[id].get())->AppendText(data.c_str());
        }
    }
};

StyleVarValueRef ReactImgui::GetStyleVar(const ImGuiStyleVar key) {
    StyleVarValueRef value;

    switch(key) {
        case ImGuiStyleVar_Alpha: value.emplace<const float*>(&m_baseStyle.Alpha); break;
        case ImGuiStyleVar_DisabledAlpha: value.emplace<const float*>(&m_baseStyle.DisabledAlpha); break;
        case ImGuiStyleVar_WindowPadding: value.emplace<const ImVec2*>(&m_baseStyle.WindowPadding); break;
        case ImGuiStyleVar_WindowRounding: value.emplace<const float*>(&m_baseStyle.WindowRounding); break;
        case ImGuiStyleVar_WindowBorderSize: value.emplace<const float*>(&m_baseStyle.WindowBorderSize); break;
        case ImGuiStyleVar_WindowMinSize: value.emplace<const ImVec2*>(&m_baseStyle.WindowMinSize); break;
        case ImGuiStyleVar_WindowTitleAlign: value.emplace<const ImVec2*>(&m_baseStyle.WindowTitleAlign); break;
        case ImGuiStyleVar_ChildRounding: value.emplace<const float*>(&m_baseStyle.ChildRounding); break;
        case ImGuiStyleVar_ChildBorderSize: value.emplace<const float*>(&m_baseStyle.ChildBorderSize); break;
        case ImGuiStyleVar_PopupRounding: value.emplace<const float*>(&m_baseStyle.PopupRounding); break;
        case ImGuiStyleVar_PopupBorderSize: value.emplace<const float*>(&m_baseStyle.PopupBorderSize); break;
        case ImGuiStyleVar_FramePadding: value.emplace<const ImVec2*>(&m_baseStyle.FramePadding); break;
        case ImGuiStyleVar_FrameRounding: value.emplace<const float*>(&m_baseStyle.FrameRounding); break;
        case ImGuiStyleVar_FrameBorderSize: value.emplace<const float*>(&m_baseStyle.FrameBorderSize); break;
        case ImGuiStyleVar_ItemSpacing: value.emplace<const ImVec2*>(&m_baseStyle.ItemSpacing); break;
        case ImGuiStyleVar_ItemInnerSpacing: value.emplace<const ImVec2*>(&m_baseStyle.ItemInnerSpacing); break;
        case ImGuiStyleVar_IndentSpacing: value.emplace<const float*>(&m_baseStyle.IndentSpacing); break;
        case ImGuiStyleVar_CellPadding: value.emplace<const ImVec2*>(&m_baseStyle.CellPadding); break;
        case ImGuiStyleVar_ScrollbarSize: value.emplace<const float*>(&m_baseStyle.ScrollbarSize); break;
        case ImGuiStyleVar_ScrollbarRounding: value.emplace<const float*>(&m_baseStyle.ScrollbarRounding); break;
        case ImGuiStyleVar_GrabMinSize: value.emplace<const float*>(&m_baseStyle.GrabMinSize); break;
        case ImGuiStyleVar_GrabRounding: value.emplace<const float*>(&m_baseStyle.GrabRounding); break;
        case ImGuiStyleVar_TabRounding: value.emplace<const float*>(&m_baseStyle.TabRounding); break;
        case ImGuiStyleVar_TabBorderSize: value.emplace<const float*>(&m_baseStyle.TabBorderSize); break;
        case ImGuiStyleVar_TabBarBorderSize: value.emplace<const float*>(&m_baseStyle.TabBarBorderSize); break;
        case ImGuiStyleVar_TableAngledHeadersAngle: value.emplace<const float*>(&m_baseStyle.TableAngledHeadersAngle); break;
        case ImGuiStyleVar_TableAngledHeadersTextAlign: value.emplace<const ImVec2*>(&m_baseStyle.TableAngledHeadersTextAlign); break;
        case ImGuiStyleVar_ButtonTextAlign: value.emplace<const ImVec2*>(&m_baseStyle.ButtonTextAlign); break;
        case ImGuiStyleVar_SelectableTextAlign: value.emplace<const ImVec2*>(&m_baseStyle.SelectableTextAlign); break;
        case ImGuiStyleVar_SeparatorTextBorderSize: value.emplace<const float*>(&m_baseStyle.SeparatorTextBorderSize); break;
        case ImGuiStyleVar_SeparatorTextAlign: value.emplace<const ImVec2*>(&m_baseStyle.SeparatorTextAlign); break;
        case ImGuiStyleVar_SeparatorTextPadding: value.emplace<const ImVec2*>(&m_baseStyle.SeparatorTextPadding); break;
        default: break;
    }

    return value;
};

ImFont* ReactImgui::GetWidgetFont(const StyledWidget* widget) {
    if (widget->HasCustomStyles() && widget->HasCustomFont(this)) {
        return m_loadedFonts[widget->m_style.value()->maybeFontIndex.value()];
    }

    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Return default font size as we might be in the middle of rendering a widget with a custom font
    return io.FontDefault;
}

float ReactImgui::GetWidgetFontSize(const StyledWidget* widget) {
    if (widget->HasCustomStyles() && widget->HasCustomFont(this)) {
        return m_loadedFonts[widget->m_style.value()->maybeFontIndex.value()]->FontSize;
    }

    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Return default font size as we might be in the middle of rendering a widget with a custom font
    return io.FontDefault->FontSize;
}

float ReactImgui::GetTextLineHeight(const StyledWidget* widget) {
    return GetWidgetFontSize(widget);
};

float ReactImgui::GetTextLineHeightWithSpacing(const StyledWidget* widget) {
    auto fontSize = GetWidgetFontSize(widget);

    float itemSpacingY = m_baseStyle.ItemSpacing.y;

    if (widget->HasCustomStyles() && widget->HasCustomStyleVar(ImGuiStyleVar_ItemSpacing)) {
        auto maybeCustomItemSpacing = widget->GetCustomStyleVar(ImGuiStyleVar_ItemSpacing);
        if (std::holds_alternative<ImVec2>(maybeCustomItemSpacing)) {
            itemSpacingY = std::get<ImVec2>(maybeCustomItemSpacing).y;
        }
    }

    return fontSize + itemSpacingY;
};

float ReactImgui::GetFrameHeight(const StyledWidget* widget) {
    auto fontSize = GetWidgetFontSize(widget);

    float framePaddingY = m_baseStyle.FramePadding.y;

    if (widget->HasCustomStyles() && widget->HasCustomStyleVar(ImGuiStyleVar_FramePadding)) {
        auto maybeCustomFramePadding = widget->GetCustomStyleVar(ImGuiStyleVar_FramePadding);
        if (std::holds_alternative<ImVec2>(maybeCustomFramePadding)) {
            framePaddingY = std::get<ImVec2>(maybeCustomFramePadding).y;
        }
    }

    return fontSize + framePaddingY * 2.0f;
};

float ReactImgui::GetFrameHeightWithSpacing(const StyledWidget* widget) {
    auto fontSize = GetWidgetFontSize(widget);

    float framePaddingY = m_baseStyle.FramePadding.y;
    float itemSpacingY = m_baseStyle.ItemSpacing.y;

    if (widget->HasCustomStyles()) {
        if (widget->HasCustomStyleVar(ImGuiStyleVar_FramePadding)) {
            auto maybeCustomFramePadding = widget->GetCustomStyleVar(ImGuiStyleVar_FramePadding);
            if (std::holds_alternative<ImVec2>(maybeCustomFramePadding)) {
                framePaddingY = std::get<ImVec2>(maybeCustomFramePadding).y;
            }
        }

        if (widget->HasCustomStyleVar(ImGuiStyleVar_ItemSpacing)) {
            auto maybeCustomItemSpacing = widget->GetCustomStyleVar(ImGuiStyleVar_ItemSpacing);
            if (std::holds_alternative<ImVec2>(maybeCustomItemSpacing)) {
                itemSpacingY = std::get<ImVec2>(maybeCustomItemSpacing).y;
            }
        }
    }

    return fontSize + framePaddingY * 2.0f + itemSpacingY;
};

ImVec2 ReactImgui::CalcTextSize(const StyledWidget* widget, const char* text, const char* text_end, bool hide_text_after_double_hash, float wrap_width)
{
    auto font = GetWidgetFont(widget);

    const char* text_display_end;
    if (hide_text_after_double_hash)
        text_display_end = ImGui::FindRenderedTextEnd(text, text_end);      // Hide anything after a '##' string
    else
        text_display_end = text_end;

    const float font_size = font->FontSize;
    if (text == text_display_end)
        return ImVec2(0.0f, font_size);
    ImVec2 text_size = font->CalcTextSizeA(font->FontSize, FLT_MAX, wrap_width, text, text_display_end, NULL);

    // Round
    // FIXME: This has been here since Dec 2015 (7b0bf230) but down the line we want this out.
    // FIXME: Investigate using ceilf or e.g.
    // - https://git.musl-libc.org/cgit/musl/tree/src/math/ceilf.c
    // - https://embarkstudios.github.io/rust-gpu/api/src/libm/math/ceilf.rs.html
    text_size.x = IM_TRUNC(text_size.x + 0.99999f);

    return text_size;
}
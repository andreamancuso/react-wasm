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

#include "reactimgui.h"
#include "implotview.h"
#include "widget.h"

using json = nlohmann::json;

ReactImgui::ReactImgui(
    const char* newWindowId, 
    const char* newGlWindowTitle
) : ImPlotView(newWindowId, newGlWindowTitle) {
    

    // bindRendererFunctions();
    SetUpFloatFormatChars();
}

void ReactImgui::RenderWidgetById(int id) {
    m_widgets[id]->Render(this);
};

void ReactImgui::RenderWidgets(int id) {
    if (m_widgets.contains(id)) {
        RenderWidgetById(id);
    }

    if (!m_widgets.contains(id) || m_widgets[id]->m_handlesChildrenWithinRenderMethod == false) {
        RenderChildren(id);
    }
};

void ReactImgui::RenderChildren(int id) {
    if (m_hierarchy.contains(id)) {
        if (m_hierarchy[id].size() > 0) {
            for (auto& childId : m_hierarchy[id]) {
                RenderWidgets(childId);
            }
        }
    }
};

void ReactImgui::InitWidget(const json& widgetDef) {
    std::string type = widgetDef["type"].template get<std::string>();
    int id = widgetDef["id"].template get<int>();

    m_widgets_mutex.lock();
    m_hierarchy_mutex.lock();

    if (type == "Combo") {
        m_widgets[id] = Combo::makeWidget(widgetDef);
    } else if (type == "Slider") {
        m_widgets[id] = Slider::makeWidget(widgetDef);
    } else if (type == "InputText") {
        m_widgets[id] = InputText::makeWidget(widgetDef);
    } else if (type == "MultiSlider") {
        m_widgets[id] = MultiSlider::makeWidget(widgetDef);
    } else if (type == "Checkbox") {
        m_widgets[id] = Checkbox::makeWidget(widgetDef);
    } else if (type == "Button") {
        m_widgets[id] = Button::makeWidget(widgetDef);
    } else if (type == "Fragment") {
        m_widgets[id] = Widget::makeWidget<Fragment>(widgetDef);
    } else if (type == "SameLine") {
        m_widgets[id] = Widget::makeWidget<SameLine>(widgetDef);
    } else if (type == "Separator") {
        m_widgets[id] = Widget::makeWidget<Separator>(widgetDef);
    } else if (type == "Indent") {
        m_widgets[id] = Widget::makeWidget<Indent>(widgetDef);
    } else if (type == "Unindent") {
        m_widgets[id] = Widget::makeWidget<Unindent>(widgetDef);
    } else if (type == "SeparatorText") {
        std::string label = widgetDef["label"].template get<std::string>();
        m_widgets[id] = std::make_unique<SeparatorText>(id, label);
    } else if (type == "BulletText") {
        std::string text = widgetDef["text"].template get<std::string>();
        m_widgets[id] = std::make_unique<BulletText>(id, text);
    } else if (type == "UnformattedText") {
        std::string text = widgetDef["text"].template get<std::string>();
        m_widgets[id] = std::make_unique<UnformattedText>(id, text);
    } else if (type == "DisabledText") {
        std::string text = widgetDef["text"].template get<std::string>();
        m_widgets[id] = std::make_unique<DisabledText>(id, text);
    } else if (type == "TabBar") {
        m_widgets[id] = Widget::makeWidget<TabBar>(widgetDef);
    } else if (type == "TabItem") {
        std::string label = widgetDef["label"].template get<std::string>();
        m_widgets[id] = std::make_unique<TabItem>(id, label);
    } else if (type == "CollapsingHeader") {
        std::string label = widgetDef["label"].template get<std::string>();
        m_widgets[id] = std::make_unique<CollapsingHeader>(id, label);
    } else if (type == "TextWrap") {
        double width = widgetDef["width"].template get<double>();
        m_widgets[id] = std::make_unique<TextWrap>(id, width);
    } else if (type == "ItemTooltip") {
        m_widgets[id] = Widget::makeWidget<ItemTooltip>(widgetDef);
    } else if (type == "TreeNode") {
        std::string label = widgetDef["label"].template get<std::string>();
        m_widgets[id] = std::make_unique<TreeNode>(id, label);
    }

    m_hierarchy[id] = std::vector<int>();

    m_widgets_mutex.unlock();
    m_hierarchy_mutex.unlock();
};

void ReactImgui::SetEventHandlers(
    OnTextChangedCallback onInputTextChangeFn,
    OnComboChangedCallback onComboChangeFn,
    OnNumericValueChangedCallback onNumericValueChangeFn,
    OnMultipleNumericValuesChangedCallback onMultiValueChangeFn,
    OnBooleanValueChangedCallback onBooleanValueChangeFn,
    OnClickCallback onClickFn
) {
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

#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = nullptr;
#endif

#ifndef IMGUI_DISABLE_FILE_FUNCTIONS
    //io.Fonts->AddFontFromFileTTF("fonts/segoeui.ttf", 18.0f);
    io.Fonts->AddFontFromFileTTF("fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("fonts/ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);
#endif

    // ImGui::StyleColorsLight();
    ImGui::StyleColorsDark();
};

void ReactImgui::Render(int window_width, int window_height) {
    SetCurrentContext();

    // ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplWGPU_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    m_widgets_mutex.lock();
    m_hierarchy_mutex.lock();

    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(window_width, window_height));

    ImGui::Begin(m_windowId, NULL, m_window_flags);

    RenderWidgets();

    ImGui::End();
    ImGui::Render();

    m_widgets_mutex.unlock();
    m_hierarchy_mutex.unlock();
};

void ReactImgui::SetWidget(std::string widgetJsonAsString) {
    InitWidget(json::parse(widgetJsonAsString));
};

void ReactImgui::PatchWidget(int id, std::string widgetJsonAsString) {
    m_widgets_mutex.lock();

    if (m_widgets.contains(id)) {
        auto widgetDef = json::parse(widgetJsonAsString);
        auto pWidget = m_widgets[id].get();

        pWidget->Patch(widgetDef);
    }

    m_widgets_mutex.unlock();
};

void ReactImgui::SetChildren(int id, std::vector<int> childrenIds) {
    m_hierarchy_mutex.lock();
    m_hierarchy[id] = childrenIds;
    m_hierarchy_mutex.unlock();
};

void ReactImgui::AppendChild(int parentId, int childId) {
    if (m_hierarchy.contains(parentId)) {
        if ( std::find(m_hierarchy[parentId].begin(), m_hierarchy[parentId].end(), childId) == m_hierarchy[parentId].end() ) {
            m_hierarchy_mutex.lock();
            m_hierarchy[parentId].push_back(childId);
            m_hierarchy_mutex.unlock();
        }
    }
};

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
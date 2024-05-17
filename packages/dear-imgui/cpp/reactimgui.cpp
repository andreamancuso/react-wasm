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

    m_hierarchy[id] = std::vector<int>();

    if (type == "InputText") {
        InitInputText(widgetDef);
    } else if (type == "Combo") {
        InitBasicCombo(widgetDef);
    } else if (type == "Slider") {
        InitSlider(widgetDef);
    } else if (type == "MultiSlider") {
        InitMultiSlider(widgetDef);
    } else if (type == "Checkbox") {
        InitCheckbox(widgetDef);
    } else if (type == "Button") {
        InitButton(widgetDef);
    } else if (type == "Fragment") {
        m_widgets[id] = std::make_unique<Fragment>(id);
    } else if (type == "SameLine") {
        m_widgets[id] = std::make_unique<SameLine>(id);
    } else if (type == "Separator") {
        m_widgets[id] = std::make_unique<Separator>(id);
    } else if (type == "Indent") {
        m_widgets[id] = std::make_unique<Indent>(id);
    } else if (type == "Unindent") {
        m_widgets[id] = std::make_unique<Indent>(id);
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
        m_widgets[id] = std::make_unique<TabBar>(id);
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
        m_widgets[id] = std::make_unique<ItemTooltip>(id);
    } else if (type == "TreeNode") {
        std::string label = widgetDef["label"].template get<std::string>();
        m_widgets[id] = std::make_unique<TreeNode>(id, label);
    }
};

void ReactImgui::InitButton(const json& val) {
    if (!val.contains("id") || !val["id"].is_number_integer()) {
        // throw?
    }

    auto id = val["id"].template get<int>();
    auto label = val.contains("label") && val["label"].is_string() ? val["label"].template get<std::string>() : "";

    if (m_widgets.contains(id)) {
        auto pButton = static_cast<Button*>(m_widgets[id].get());

        pButton->m_label = label;
    } else {
        m_widgets[id] = Button::makeButtonWidget(id, label);
    }
};

void ReactImgui::InitCheckbox(const json& val) {
    if (!val.contains("id") || !val["id"].is_number_integer()) {
        // throw?
    }

    auto id = val["id"].template get<int>();
    auto defaultChecked = val.contains("defaultChecked") && val["defaultChecked"].is_boolean() ? val["defaultChecked"].template get<bool>() : false;
    auto label = val.contains("label") && val["label"].is_string() ? val["label"].template get<std::string>() : "";

    if (m_widgets.contains(id)) {
        auto pCheckbox = static_cast<Checkbox*>(m_widgets[id].get());

        pCheckbox->m_label = label;
    } else {
        m_widgets[id] = Checkbox::makeCheckboxWidget(id, label, defaultChecked);
    }
};

void ReactImgui::InitSlider(const json& val) {
    if (!val.contains("id") || !val["id"].is_number_integer()) {
        // throw?
    }

    auto id = val["id"].template get<int>();
    auto defaultValue = val.contains("defaultValue") && val["defaultValue"].is_number() ? val["defaultValue"].template get<float>() : 0.0f;
    auto min = val.contains("min") && val["min"].is_number() ? val["min"].template get<float>() : 0.0f;
    auto max = val.contains("max") && val["max"].is_number() ? val["max"].template get<float>() : 10.0f;
    auto label = val.contains("label") && val["label"].is_string() ? val["label"].template get<std::string>() : "";
    auto sliderType = val.contains("sliderType") && val["sliderType"].is_string() ? val["sliderType"].template get<std::string>() : "default";

    if (m_widgets.contains(id)) {
        auto pSlider = static_cast<Slider*>(m_widgets[id].get());

        pSlider->m_label = label;
        
        if (pSlider->m_value > max) {
            // Don't want to go out of bounds!
            pSlider->m_value = max;
        }

        pSlider->m_min = min;
        pSlider->m_max = max;
    } else {
        m_widgets[id] = Slider::makeSliderWidget(id, label, defaultValue, min, max, sliderType);
    }
};

void ReactImgui::InitMultiSlider(const json& val) {
    if (!val.contains("id") || !val["id"].is_number_integer()) {
        // throw?
    }

    auto id = val["id"].template get<int>();
    auto numValues = val.contains("numValues") && val["numValues"].is_number() ? val["numValues"].template get<int>() : 2;
    auto decimalDigits = val.contains("decimalDigits") && val["decimalDigits"].is_number() ? val["decimalDigits"].template get<int>() : 0;
    
    auto min = val.contains("min") && val["min"].is_number() ? val["min"].template get<float>() : 0.0f;
    auto max = val.contains("max") && val["max"].is_number() ? val["max"].template get<float>() : 10.0f;
    auto label = val.contains("label") && val["label"].is_string() ? val["label"].template get<std::string>() : "";

    if (m_widgets.contains(id)) {
        auto pMultiSlider = static_cast<MultiSlider*>(m_widgets[id].get());

        pMultiSlider->m_label = label;
        
        // todo: Changing numValues should probably throw - or emit a warning?

        pMultiSlider->m_min = min;
        pMultiSlider->m_max = max;
        pMultiSlider->m_decimalDigits = decimalDigits;
    } else {
        if (val.contains("defaultValues") && val["defaultValues"].is_array() && val["defaultValues"].size() == numValues) {
            m_widgets[id] = MultiSlider::makeMultiSliderWidget(id, label, min, max, numValues, decimalDigits, val["defaultValues"]);
        } else {
            m_widgets[id] = MultiSlider::makeMultiSliderWidget(id, label, min, max, numValues, decimalDigits);
        }
    }
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

void ReactImgui::InitInputText(const json& val) {
    if (!val.contains("id") || !val["id"].is_number_integer()) {
        // throw?
    }

    auto id = val["id"].template get<int>();
    auto defaultValue = val.contains("defaultValue") && val["defaultValue"].is_string() ? val["defaultValue"].template get<std::string>() : "";
    auto label = val.contains("label") && val["label"].is_string() ? val["label"].template get<std::string>() : "";

    if (m_widgets.contains(id)) {
        auto pInputText = static_cast<InputText*>(m_widgets[id].get());

        pInputText->m_label = label;
    } else {
        m_widgets[id] = InputText::makeInputTextWidget(id, defaultValue, label);
    }
};

void ReactImgui::InitBasicCombo(const json& val) {
    if (!val.contains("id") || !val["id"].is_number_integer()) {
        // throw?
    }

    auto id = val["id"].template get<int>();
    auto defaultValue = val.contains("defaultValue") && val["defaultValue"].is_number() ? val["defaultValue"].template get<int>() : 0;
    auto label = val["label"].template get<std::string>();
    
    if (m_widgets.contains(id)) {
        auto pCombo = static_cast<Combo*>(m_widgets[id].get());

        pCombo->m_label = label;
    } else {
        m_widgets[id] = Combo::makeComboWidget(id, label, defaultValue, val["optionsList"].template get<std::string>());
    }
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

    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(window_width, window_height));

    ImGui::Begin(m_windowId, NULL, m_window_flags);

    RenderWidgets();

    ImGui::End();
    ImGui::Render();
};

void ReactImgui::SetWidget(std::string widgetJsonAsString) {
    InitWidget(json::parse(widgetJsonAsString));
};

void ReactImgui::PatchWidget(int id, std::string widgetJsonAsString) {
    if (m_widgets.contains(id)) {
        auto widgetDef = json::parse(widgetJsonAsString);
        auto pWidget = m_widgets[id].get();
        auto type = pWidget->m_type;

        if (type == "InputText") {
            if (widgetDef.contains("label") && widgetDef["label"].is_string()) {
                static_cast<InputText*>(pWidget)->m_label = widgetDef["label"].template get<std::string>();
            }
        } else if (type == "Combo") {
            if (widgetDef.contains("label") && widgetDef["label"].is_string()) {
                static_cast<Combo*>(pWidget)->m_label = widgetDef["label"].template get<std::string>();
            }
        } else if (type == "Slider") {
            if (widgetDef.contains("label") && widgetDef["label"].is_string()) {
                static_cast<Slider*>(pWidget)->m_label = widgetDef["label"].template get<std::string>();
            }
        } else if (type == "MultiSlider") {
            if (widgetDef.contains("label") && widgetDef["label"].is_string()) {
                static_cast<MultiSlider*>(pWidget)->m_label = widgetDef["label"].template get<std::string>();
            }
        } else if (type == "Checkbox") {
            if (widgetDef.contains("label") && widgetDef["label"].is_string()) {
                static_cast<Checkbox*>(pWidget)->m_label = widgetDef["label"].template get<std::string>();
            }
        } else if (type == "Button") {
            if (widgetDef.contains("label") && widgetDef["label"].is_string()) {
                static_cast<Button*>(pWidget)->m_label = widgetDef["label"].template get<std::string>();
            }
        } else if (type == "Fragment") {
            
        } else if (type == "SameLine") {
            
        } else if (type == "Separator") {
            
        } else if (type == "Indent") {
            
        } else if (type == "Unindent") {
            
        } else if (type == "SeparatorText") {
            if (widgetDef.contains("label") && widgetDef["label"].is_string()) {
                static_cast<SeparatorText*>(pWidget)->m_label = widgetDef["label"].template get<std::string>();
            }
        } else if (type == "BulletText") {
            if (widgetDef.contains("text") && widgetDef["text"].is_string()) {
                static_cast<BulletText*>(pWidget)->m_text = widgetDef["text"].template get<std::string>();
            }
        } else if (type == "UnformattedText") {
            if (widgetDef.contains("text") && widgetDef["text"].is_string()) {
                static_cast<UnformattedText*>(pWidget)->m_text = widgetDef["text"].template get<std::string>();
            }
        } else if (type == "DisabledText") {
            if (widgetDef.contains("text") && widgetDef["text"].is_string()) {
                static_cast<DisabledText*>(pWidget)->m_text = widgetDef["text"].template get<std::string>();
            }
        } else if (type == "TabBar") {
            
        } else if (type == "TabItem") {
            if (widgetDef.contains("label") && widgetDef["label"].is_string()) {
                static_cast<TabItem*>(pWidget)->m_label = widgetDef["label"].template get<std::string>();
            }
        } else if (type == "CollapsingHeader") {
            if (widgetDef.contains("label") && widgetDef["label"].is_string()) {
                static_cast<CollapsingHeader*>(pWidget)->m_label = widgetDef["label"].template get<std::string>();
            }
        } else if (type == "TextWrap") {
            if (widgetDef.contains("width") && widgetDef["width"].is_number()) {
                static_cast<TextWrap*>(pWidget)->m_width = widgetDef["width"].template get<double>();
            }
        } else if (type == "ItemTooltip") {
            
        } else if (type == "TreeNode") {
            if (widgetDef.contains("label") && widgetDef["label"].is_string()) {
                static_cast<TreeNode*>(pWidget)->m_label = widgetDef["label"].template get<std::string>();
            }
        }
    }
};

void ReactImgui::SetChildren(int id, std::vector<int> childrenIds) {
    m_hierarchy[id] = childrenIds;
};

void ReactImgui::AppendChild(int parentId, int childId) {
    if (m_hierarchy.contains(parentId)) {
        if ( std::find(m_hierarchy[parentId].begin(), m_hierarchy[parentId].end(), childId) == m_hierarchy[parentId].end() ) {
            m_hierarchy[parentId].push_back(childId);
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
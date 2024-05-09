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
    widgets[id]->Render(this);
};

void ReactImgui::RenderWidgets(int id) {
    if (this->widgets.contains(id)) {
        // printf("Rendering widget #%d\n", id);
        RenderWidgetById(id);
    }

    if (!this->widgets.contains(id) || this->widgets[id]->handlesChildrenWithinRenderMethod == false) {
        RenderChildren(id);
    }
};

void ReactImgui::RenderChildren(int id) {
    if (this->hierarchy.contains(id)) {
        if (this->hierarchy[id].size() > 0) {
            // printf("About to render %lu children for widget #%d\n", this->hierarchy[id].size(), id);
            auto childrenIds = this->hierarchy[id];

            for (auto& childId : childrenIds) {
                // printf("About to render child widget #%d of widget #%d\n", childId, id);
                this->RenderWidgets(childId);
            }
        }
    }
};

void ReactImgui::InitWidget(const json& widgetDef) {
    std::string type = widgetDef["type"].template get<std::string>();
    int id = widgetDef["id"].template get<int>();

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
        widgets[id] = std::make_unique<Fragment>(id);
    } else if (type == "SameLine") {
        widgets[id] = std::make_unique<SameLine>(id);
    } else if (type == "Separator") {
        widgets[id] = std::make_unique<Separator>(id);
    } else if (type == "Indent") {
        widgets[id] = std::make_unique<Indent>(id);
    } else if (type == "Unindent") {
        widgets[id] = std::make_unique<Indent>(id);
    } else if (type == "SeparatorText") {
        std::string label = widgetDef["label"].template get<std::string>();
        widgets[id] = std::make_unique<SeparatorText>(id, label);
    } else if (type == "BulletText") {
        std::string text = widgetDef["text"].template get<std::string>();
        widgets[id] = std::make_unique<BulletText>(id, text);
    } else if (type == "UnformattedText") {
        std::string text = widgetDef["text"].template get<std::string>();
        widgets[id] = std::make_unique<UnformattedText>(id, text);
    } else if (type == "DisabledText") {
        std::string text = widgetDef["text"].template get<std::string>();
        widgets[id] = std::make_unique<DisabledText>(id, text);
    } else if (type == "TabBar") {
        widgets[id] = std::make_unique<TabBar>(id);
    } else if (type == "TabItem") {
        std::string label = widgetDef["label"].template get<std::string>();
        widgets[id] = std::make_unique<TabItem>(id, label);
    } else if (type == "CollapsingHeader") {
        std::string label = widgetDef["label"].template get<std::string>();
        widgets[id] = std::make_unique<CollapsingHeader>(id, label);
    } else if (type == "TextWrap") {
        double width = widgetDef["width"].template get<double>();
        widgets[id] = std::make_unique<TextWrap>(id, width);
    } else if (type == "ItemTooltip") {
        widgets[id] = std::make_unique<ItemTooltip>(id);
    } else if (type == "TreeNode") {
        std::string label = widgetDef["label"].template get<std::string>();
        widgets[id] = std::make_unique<TreeNode>(id, label);
    }
};

void ReactImgui::InitButton(const json& val) {
    if (!val.contains("id") || !val["id"].is_number_integer()) {
        // throw?
    }

    auto id = val["id"].template get<int>();
    auto label = val.contains("label") && val["label"].is_string() ? val["label"].template get<std::string>() : "";

    if (widgets.contains(id)) {
        auto pButton = static_cast<Button*>(widgets[id].get());

        pButton->label = label;
    } else {
        widgets[id] = Button::makeButtonWidget(id, label);
    }
};

void ReactImgui::InitCheckbox(const json& val) {
    if (!val.contains("id") || !val["id"].is_number_integer()) {
        // throw?
    }

    auto id = val["id"].template get<int>();
    auto defaultChecked = val.contains("defaultChecked") && val["defaultChecked"].is_boolean() ? val["defaultChecked"].template get<bool>() : false;
    auto label = val.contains("label") && val["label"].is_string() ? val["label"].template get<std::string>() : "";

    if (widgets.contains(id)) {
        auto pCheckbox = static_cast<Checkbox*>(widgets[id].get());

        pCheckbox->label = label;
    } else {
        widgets[id] = Checkbox::makeCheckboxWidget(id, label, defaultChecked);
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

    if (widgets.contains(id)) {
        auto pSlider = static_cast<Slider*>(widgets[id].get());

        pSlider->label = label;
        
        if (pSlider->value > max) {
            // Don't want to go out of bounds!
            pSlider->value = max;
        }

        pSlider->min = min;
        pSlider->max = max;
    } else {
        widgets[id] = Slider::makeSliderWidget(id, label, defaultValue, min, max, sliderType);
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

    if (widgets.contains(id)) {
        auto pMultiSlider = static_cast<MultiSlider*>(widgets[id].get());

        pMultiSlider->label = label;
        
        // todo: Changing numValues should probably throw - or emit a warning?

        pMultiSlider->min = min;
        pMultiSlider->max = max;
        pMultiSlider->decimalDigits = decimalDigits;
    } else {
        if (val.contains("defaultValues") && val["defaultValues"].is_array() && val["defaultValues"].size() == numValues) {
            widgets[id] = MultiSlider::makeMultiSliderWidget(id, label, min, max, numValues, decimalDigits, val["defaultValues"]);
        } else {
            widgets[id] = MultiSlider::makeMultiSliderWidget(id, label, min, max, numValues, decimalDigits);
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
    onInputTextChange = onInputTextChangeFn;
    onComboChange = onComboChangeFn;
    onNumericValueChange = onNumericValueChangeFn;
    onMultiValueChange = onMultiValueChangeFn;
    onBooleanValueChange = onBooleanValueChangeFn;
    onClick = onClickFn;

    Widget::onInputTextChange_ = onInputTextChangeFn;
};

void ReactImgui::InitInputText(const json& val) {
    if (!val.contains("id") || !val["id"].is_number_integer()) {
        // throw?
    }

    auto id = val["id"].template get<int>();
    auto defaultValue = val.contains("defaultValue") && val["defaultValue"].is_string() ? val["defaultValue"].template get<std::string>() : "";
    auto label = val.contains("label") && val["label"].is_string() ? val["label"].template get<std::string>() : "";

    if (widgets.contains(id)) {
        auto pInputText = static_cast<InputText*>(widgets[id].get());

        pInputText->label = label;
    } else {
        widgets[id] = InputText::makeInputTextWidget(id, defaultValue, label);
    }
};

void ReactImgui::InitBasicCombo(const json& val) {
    if (!val.contains("id") || !val["id"].is_number_integer()) {
        // throw?
    }

    auto id = val["id"].template get<int>();
    auto defaultValue = val.contains("defaultValue") && val["defaultValue"].is_number() ? val["defaultValue"].template get<int>() : 0;
    auto label = val["label"].template get<std::string>();
    
    if (widgets.contains(id)) {
        auto pCombo = static_cast<Combo*>(widgets[id].get());

        pCombo->label = label;
    } else {
        widgets[id] = Combo::makeComboWidget(id, label, defaultValue, val["optionsList"].template get<std::string>());
    }
};

void ReactImgui::SetUpFloatFormatChars() {
    floatFormatChars[0] = std::make_unique<char[]>(4);
    floatFormatChars[1] = std::make_unique<char[]>(4);
    floatFormatChars[2] = std::make_unique<char[]>(4);
    floatFormatChars[3] = std::make_unique<char[]>(4);
    floatFormatChars[4] = std::make_unique<char[]>(4);
    floatFormatChars[5] = std::make_unique<char[]>(4);
    floatFormatChars[6] = std::make_unique<char[]>(4);
    floatFormatChars[7] = std::make_unique<char[]>(4);
    floatFormatChars[8] = std::make_unique<char[]>(4);
    floatFormatChars[9] = std::make_unique<char[]>(4);

    strcpy(floatFormatChars[0].get(), "%.0f");
    strcpy(floatFormatChars[1].get(), "%.1f");
    strcpy(floatFormatChars[2].get(), "%.2f");
    strcpy(floatFormatChars[3].get(), "%.3f");
    strcpy(floatFormatChars[4].get(), "%.4f");
    strcpy(floatFormatChars[5].get(), "%.5f");
    strcpy(floatFormatChars[6].get(), "%.6f");
    strcpy(floatFormatChars[7].get(), "%.7f");
    strcpy(floatFormatChars[8].get(), "%.8f");
    strcpy(floatFormatChars[9].get(), "%.9f");
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

    ImGui::Begin(windowId, NULL, window_flags);

    RenderWidgets();

    ImGui::End();
    ImGui::Render();
};

void ReactImgui::SetWidget(std::string widgetJsonAsString) {
    InitWidget(json::parse(widgetJsonAsString));
};

void ReactImgui::PatchWidget(int id, std::string widgetJsonAsString) {
    if (this->widgets.contains(id)) {
        auto widgetDef = json::parse(widgetJsonAsString);
        auto pWidget = widgets[id].get();
        auto type = pWidget->type;

        if (type == "InputText") {
            if (widgetDef.contains("label") && widgetDef["label"].is_string()) {
                static_cast<InputText*>(pWidget)->label = widgetDef["label"].template get<std::string>();
            }
        } else if (type == "Combo") {
            if (widgetDef.contains("label") && widgetDef["label"].is_string()) {
                static_cast<Combo*>(pWidget)->label = widgetDef["label"].template get<std::string>();
            }
        } else if (type == "Slider") {
            if (widgetDef.contains("label") && widgetDef["label"].is_string()) {
                static_cast<Slider*>(pWidget)->label = widgetDef["label"].template get<std::string>();
            }
        } else if (type == "MultiSlider") {
            if (widgetDef.contains("label") && widgetDef["label"].is_string()) {
                static_cast<MultiSlider*>(pWidget)->label = widgetDef["label"].template get<std::string>();
            }
        } else if (type == "Checkbox") {
            if (widgetDef.contains("label") && widgetDef["label"].is_string()) {
                static_cast<Checkbox*>(pWidget)->label = widgetDef["label"].template get<std::string>();
            }
        } else if (type == "Button") {
            if (widgetDef.contains("label") && widgetDef["label"].is_string()) {
                static_cast<Button*>(pWidget)->label = widgetDef["label"].template get<std::string>();
            }
        } else if (type == "Fragment") {
            
        } else if (type == "SameLine") {
            
        } else if (type == "Separator") {
            
        } else if (type == "Indent") {
            
        } else if (type == "Unindent") {
            
        } else if (type == "SeparatorText") {
            if (widgetDef.contains("label") && widgetDef["label"].is_string()) {
                static_cast<SeparatorText*>(pWidget)->label = widgetDef["label"].template get<std::string>();
            }
        } else if (type == "BulletText") {
            if (widgetDef.contains("text") && widgetDef["text"].is_string()) {
                static_cast<BulletText*>(pWidget)->text = widgetDef["text"].template get<std::string>();
            }
        } else if (type == "UnformattedText") {
            if (widgetDef.contains("text") && widgetDef["text"].is_string()) {
                static_cast<UnformattedText*>(pWidget)->text = widgetDef["text"].template get<std::string>();
            }
        } else if (type == "DisabledText") {
            if (widgetDef.contains("text") && widgetDef["text"].is_string()) {
                static_cast<DisabledText*>(pWidget)->text = widgetDef["text"].template get<std::string>();
            }
        } else if (type == "TabBar") {
            
        } else if (type == "TabItem") {
            if (widgetDef.contains("label") && widgetDef["label"].is_string()) {
                static_cast<TabItem*>(pWidget)->label = widgetDef["label"].template get<std::string>();
            }
        } else if (type == "CollapsingHeader") {
            if (widgetDef.contains("label") && widgetDef["label"].is_string()) {
                static_cast<CollapsingHeader*>(pWidget)->label = widgetDef["label"].template get<std::string>();
            }
        } else if (type == "TextWrap") {
            if (widgetDef.contains("width") && widgetDef["width"].is_number()) {
                static_cast<TextWrap*>(pWidget)->width = widgetDef["width"].template get<double>();
            }
        } else if (type == "ItemTooltip") {
            
        } else if (type == "TreeNode") {
            if (widgetDef.contains("label") && widgetDef["label"].is_string()) {
                static_cast<TreeNode*>(pWidget)->label = widgetDef["label"].template get<std::string>();
            }
        }

    }
};

void ReactImgui::SetChildren(int id, std::vector<int> childrenIds) {
    hierarchy[id] = childrenIds;
};

std::vector<int> ReactImgui::GetChildren(int id) {
    return hierarchy[id];
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
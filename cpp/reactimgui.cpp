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

#include "implotview.cpp"

using json = nlohmann::json;

struct InputTextStuff {
    std::unique_ptr<char[]> bufferPointer;
    std::shared_ptr<emscripten::val> onInputTextChange;
    std::string widgetId;
    std::string defaultValue;
    std::string label;
};

struct ComboStuff {
    int selectedIndex;
    std::string label;
    std::unique_ptr<char[]> itemsSeparatedByZeros; // Relevant for 'basic' combo only
};

struct SliderStuff {
    float value;
    float min;
    float max;
    std::string label;
};

struct MultiSliderStuff {
    std::unique_ptr<float[]> values;
    float min;
    float max;
    int numValues;
    std::string label;
};

struct CheckboxStuff {
    bool checked;
    std::string label;
};

struct TabItemStuff {
    bool visible; // ImGui misleadingly refers to this as `p_open`
    std::string label;
};

struct ButtonStuff {
    std::string label;
};

class Combo {
    private:
        Combo() {}

        /**
         * Takes array of [{value: number; label: string}] and converts into "label\0label\0label\0" (double NULL character at the end)
         * because this is what ImGui:Combo() expects
        */
        static std::unique_ptr<char[]> getItemsSeparatedByZeros(const json& options) {
            auto optionsIterator = options.items();
            int itemsStringLength = options.size() + 1; // Account for one NULL character for each option plus the final NULL character

            for (auto it = optionsIterator.begin(); it != optionsIterator.end(); ++it) {
                itemsStringLength += strlen(it.value()["label"].template get<std::string>().c_str());
            }

            auto itemsSeparatedByZerosUniquePtr = std::make_unique<char[]>(itemsStringLength);
            auto itemsSeparatedByZeros = itemsSeparatedByZerosUniquePtr.get();

            int offset = 0;

            for (auto it = optionsIterator.begin(); it != optionsIterator.end(); ++it) {
                
                auto label = it.value()["label"].template get<std::string>().c_str();

                memcpy(&itemsSeparatedByZeros[offset], label, strlen(label) + 1);

                offset += strlen(label) + 1;
            }

            return itemsSeparatedByZerosUniquePtr;
        }

        /**
         * Takes comma-delimited string of options "label,label,label" and converts into "label\0label\0label\0" (double NULL character at the end)
         * because this is what ImGui:Combo() expects
        */
        static std::unique_ptr<char[]> getItemsSeparatedByZeros(std::string optionsList) {
            std::string delimiter = ",";

            int itemsStringLength = 1;  // Account for final NULL character
            std::vector<std::string> options;
            std::string token;
            std::stringstream input;
            input << optionsList;

            while (std::getline(input, token, ',')) {
                options.push_back(token);
                itemsStringLength += strlen(token.c_str()) + 1; // Account for NULL character
            }

            auto itemsSeparatedByZerosUniquePtr = std::make_unique<char[]>(itemsStringLength);
            auto itemsSeparatedByZeros = itemsSeparatedByZerosUniquePtr.get();
            
            int offset = 0;

            for (auto it = options.begin(); it != options.end(); ++it) {

                auto label = it->c_str();

                memcpy(&itemsSeparatedByZeros[offset], label, strlen(label) + 1);

                offset += strlen(label) + 1;
            }

            return itemsSeparatedByZerosUniquePtr;
        }

    public:
        static std::unique_ptr<ComboStuff> getComboStuff(std::string label, int defaultValue, const json& options) {
            auto comboStuff = std::make_unique<ComboStuff>();

            comboStuff->selectedIndex = defaultValue;
            comboStuff->label = label;
            comboStuff->itemsSeparatedByZeros = Combo::getItemsSeparatedByZeros(options);

            return comboStuff;
        }

        static std::unique_ptr<ComboStuff> getComboStuff(std::string label, int defaultValue, std::string optionsList) {
            auto comboStuff = std::make_unique<ComboStuff>();

            comboStuff->selectedIndex = defaultValue;
            comboStuff->label = label;
            comboStuff->itemsSeparatedByZeros = Combo::getItemsSeparatedByZeros(optionsList);

            return comboStuff;
        }
};

class ReactImgui final : public ImPlotView {
    private:
        json widgets;
        std::unordered_map<std::string, std::unique_ptr<InputTextStuff>> inputTexts;
        std::unordered_map<std::string, std::unique_ptr<ComboStuff>> combos;
        std::unordered_map<std::string, std::unique_ptr<SliderStuff>> sliders;
        std::unordered_map<std::string, std::unique_ptr<MultiSliderStuff>> multiSliders;
        std::unordered_map<std::string, std::unique_ptr<CheckboxStuff>> checkboxes;
        std::unordered_map<std::string, std::unique_ptr<ButtonStuff>> buttons;
        std::unordered_map<std::string, std::unique_ptr<TabItemStuff>> tabItems;

        std::shared_ptr<emscripten::val> onInputTextChange;
        std::unique_ptr<emscripten::val> onComboChange;
        std::unique_ptr<emscripten::val> onNumericValueChange;
        std::unique_ptr<emscripten::val> onMultiValueChange;
        std::unique_ptr<emscripten::val> onBooleanValueChange;
        std::unique_ptr<emscripten::val> onClick;

        ImGuiInputTextFlags inputTextFlags = ImGuiInputTextFlags_CallbackEdit | ImGuiInputTextFlags_NoUndoRedo;

        static int InputTextCb(ImGuiInputTextCallbackData* data)
        {
            if (data->EventFlag == ImGuiInputTextFlags_CallbackEdit) {
                auto inputTextStuff = reinterpret_cast<InputTextStuff*>(data->UserData);

                std::string str = data->Buf;
                inputTextStuff->onInputTextChange->call<void>("call", 0, inputTextStuff->widgetId, str);
            }

            return 0;
        }

        void RenderWidgets(const json& widgetsLevel) {
            for (auto& [key, val] : widgetsLevel.items()) {
                bool is_array = val.type() == nlohmann::detail::value_t::array;
                bool is_object = val.type() == nlohmann::detail::value_t::object;

                if (is_array) {
                    RenderWidgets(val);
                } else if (is_object) {
                    auto type = val["type"].template get<std::string>();

                    if (type == "Fragment") {
                        if (val.contains("children")) {
                            RenderWidgets(val["children"]);
                        }
                    } else if (type == "TabBar") {
                        ImGui::PushID(val["id"].template get<std::string>().c_str());
                        if (ImGui::BeginTabBar(val["id"].template get<std::string>().c_str(), ImGuiTabBarFlags_None)) {
                            if (val.contains("children")) {
                                RenderWidgets(val["children"]);
                            }
                            ImGui::EndTabBar();
                        }
                        ImGui::PopID();
                    } else if (type == "TabItem") {
                        RenderTabItem(val);
                    } else if (type == "TreeNode") {
                        ImGui::PushID(val["id"].template get<std::string>().c_str());
                        if (ImGui::TreeNode(val["label"].template get<std::string>().c_str())) {
                            if (val.contains("children")) {
                                RenderWidgets(val["children"]);
                            }

                            ImGui::TreePop();
                            ImGui::Spacing();
                        }
                        ImGui::PopID();
                    } else if (type == "ItemTooltip") {
                        if (ImGui::BeginItemTooltip()) {
                            if (val.contains("children")) {
                                RenderWidgets(val["children"]);
                            }

                            ImGui::EndTooltip();
                        }
                    } else if (type == "TextWrap") {
                        ImGui::PushTextWrapPos(val["width"].template get<double>());
                        if (val.contains("children")) {
                            RenderWidgets(val["children"]);
                        }
                        ImGui::PopTextWrapPos();
                    } else if (type == "CollapsingHeader") {
                        ImGui::PushID(val["id"].template get<std::string>().c_str());
                        if (ImGui::CollapsingHeader(val["label"].template get<std::string>().c_str())) {
                            if (val.contains("children")) {
                                RenderWidgets(val["children"]);
                            }
                        }
                        ImGui::PopID();
                    } else if (type == "Indent") {
                        ImGui::Indent();
                    } else if (type == "Unindent") {
                        ImGui::Unindent();
                    } else if (type == "SameLine") {
                        ImGui::SameLine();
                    } else if (type == "TextDisabled") {
                        ImGui::TextDisabled(val["text"].template get<std::string>().c_str());
                    } else if (type == "UnformattedText") {
                        ImGui::TextUnformatted(val["text"].template get<std::string>().c_str());
                    } else if (type == "BulletText") {
                        ImGui::BulletText(val["text"].template get<std::string>().c_str());
                    } else if (type == "SeparatorText") {
                        ImGui::SeparatorText(val["label"].template get<std::string>().c_str());
                    } else if (type == "Separator") {
                        ImGui::Separator();
                    } else if (type == "InputText") {
                        RenderInputText(val);
                    } else if (type == "Combo") {
                        // todo: add support for 'regular' combo
                        RenderBasicCombo(val);
                    } else if (type == "Slider") {
                        RenderSlider(val);
                    } else if (type == "Checkbox") {
                        RenderCheckbox(val);
                    } else if (type == "Button") {
                        RenderButton(val);
                    }
                }
            }
        }

        void RenderTabItem(const json& val) {
            auto id = val["id"].template get<std::string>();
            const char* idAsChar = id.c_str();

            if (tabItems.contains(id)) {
                ImGui::PushID(idAsChar);
                // when p_open is true, tab item can be closed via dedicated button
                // when p_open is false, tab item isn't even rendered
                if (ImGui::BeginTabItem(tabItems[id].get()->label.c_str())) {
                // if (ImGui::BeginTabItem(tabItems[id].get()->label.c_str(), &tabItems[id]->visible)) {
                    // todo: how to send onSelectedTabChange event?
                    // onBooleanValueChange->call<void>("call", 0, id, tabItems[id]->open);

                    if (val.contains("children")) {
                        RenderWidgets(val["children"]);
                    }

                    ImGui::EndTabItem();
                }
                ImGui::PopID();
            }
        }

        void RenderCheckbox(const json& val) {
            auto id = val["id"].template get<std::string>();
            const char* idAsChar = id.c_str();

            if (checkboxes.contains(id)) {
                ImGui::PushID(idAsChar);
                if (ImGui::Checkbox(checkboxes[id].get()->label.c_str(), &checkboxes[id]->checked)) {
                    onBooleanValueChange->call<void>("call", 0, id, checkboxes[id]->checked);
                }
                ImGui::PopID();
            }
        }

        void RenderButton(const json& val) {
            auto id = val["id"].template get<std::string>();
            const char* idAsChar = id.c_str();

            if (buttons.contains(id)) {
                ImGui::PushID(idAsChar);
                if (ImGui::Button(buttons[id].get()->label.c_str())) {
                    onClick->call<void>("call", 0, id);
                }
                ImGui::PopID();
            }
        }

        void RenderSlider(const json& val) {
            auto id = val["id"].template get<std::string>();
            const char* idAsChar = id.c_str();

            if (sliders.contains(id)) {
                ImGui::PushID(idAsChar);
                if (ImGui::SliderFloat(sliders[id].get()->label.c_str(), &sliders[id]->value, sliders[id]->min, sliders[id]->max, "%.0f")) { // min and max are not passed by reference, it's a copy every time...
                    onNumericValueChange->call<void>("call", 0, id, sliders[id]->value);
                }
                ImGui::PopID();
            }
        }

        void RenderInputText(const json& val) {
            auto id = val["id"].template get<std::string>();
            const char* idAsChar = id.c_str();

            if (inputTexts.contains(id)) {
                ImGui::PushID(idAsChar);
                ImGui::InputText(inputTexts[id].get()->label.c_str(), inputTexts[id].get()->bufferPointer.get(), 100, inputTextFlags, InputTextCb, (void*)inputTexts[id].get());
                ImGui::PopID();
            }
        }

        void RenderBasicCombo(const json& val) {
            auto id = val["id"].template get<std::string>();
            const char* idAsChar = id.c_str();

            if (combos.contains(id)) {
                ImGui::PushID(idAsChar);
                if (ImGui::Combo(combos[id].get()->label.c_str(), &combos[id].get()->selectedIndex, combos[id].get()->itemsSeparatedByZeros.get())) {
                    onComboChange->call<void>("call", 0, id, combos[id].get()->selectedIndex);
                }
                ImGui::PopID();
            }
        }

        void InitWidgets(const json& widgetsLevel) {
            for (auto& [key, val] : widgetsLevel.items()) {
                bool is_array = val.type() == nlohmann::detail::value_t::array;
                bool is_object = val.type() == nlohmann::detail::value_t::object;

                if (is_array) {
                    InitWidgets(val);
                } else if (is_object) {
                    auto type = val["type"].template get<std::string>();

                    if (val.contains("children")) {
                        InitWidgets(val["children"]);
                    }

                    if (type == "InputText") {
                        InitInputText(val);
                    } else if (type == "Combo") {
                        InitCombo(val);
                    } else if (type == "Slider") {
                        InitSlider(val);
                    } else if (type == "Checkbox") {
                        InitCheckbox(val);
                    } else if (type == "Button") {
                        InitButton(val);
                    } else if (type == "TabItem") {
                        InitTabItem(val);
                    }
                }
            }
        }

        void InitButton(const json& val) {
            if (!val.contains("id") || !val["id"].is_string()) {
                // throw?
            }

            auto id = val["id"].template get<std::string>();
            auto label = val.contains("label") && val["label"].is_string() ? val["label"].template get<std::string>() : "";

            if (buttons.contains(id)) {
                buttons[id]->label = label;
            } else {
                buttons[id] = std::make_unique<ButtonStuff>();

                buttons[id]->label = label;
            }
        }

        void InitTabItem(const json& val) {
            if (!val.contains("id") || !val["id"].is_string()) {
                // throw?
            }

            auto id = val["id"].template get<std::string>();
            // auto defaultOpen = val.contains("defaultOpen") && val["defaultOpen"].is_boolean() ? val["defaultOpen"].template get<bool>() : false;
            auto label = val.contains("label") && val["label"].is_string() ? val["label"].template get<std::string>() : "";

            if (tabItems.contains(id)) {
                tabItems[id]->label = label;
            } else {
                tabItems[id] = std::make_unique<TabItemStuff>();

                tabItems[id]->label = label;
                tabItems[id]->visible = true;
            }
        }

        void InitCheckbox(const json& val) {
            if (!val.contains("id") || !val["id"].is_string()) {
                // throw?
            }

            auto id = val["id"].template get<std::string>();
            auto defaultChecked = val.contains("defaultChecked") && val["defaultChecked"].is_boolean() ? val["defaultChecked"].template get<bool>() : false;
            auto label = val.contains("label") && val["label"].is_string() ? val["label"].template get<std::string>() : "";

            if (checkboxes.contains(id)) {
                checkboxes[id]->label = label;
            } else {
                checkboxes[id] = std::make_unique<CheckboxStuff>();

                checkboxes[id]->label = label;
                checkboxes[id]->checked = defaultChecked;
            }
        }

        void InitSlider(const json& val) {
            if (!val.contains("id") || !val["id"].is_string()) {
                // throw?
            }

            auto id = val["id"].template get<std::string>();
            auto defaultValue = val.contains("defaultValue") && val["defaultValue"].is_number() ? val["defaultValue"].template get<float>() : 0.0f;
            auto min = val.contains("min") && val["min"].is_number() ? val["min"].template get<float>() : 0.0f;
            auto max = val.contains("max") && val["max"].is_number() ? val["max"].template get<float>() : 10.0f;
            auto label = val.contains("label") && val["label"].is_string() ? val["label"].template get<std::string>() : "";

            if (sliders.contains(id)) {
                sliders[id]->label = label;
                
                if (sliders[id]->value > max) {
                    // Don't want to go out of bounds!
                    sliders[id]->value = max;
                }

                sliders[id]->min = min;
                sliders[id]->max = max;
            } else {
                sliders[id] = std::make_unique<SliderStuff>();

                sliders[id]->label = label;
                sliders[id]->value = defaultValue;
                sliders[id]->min = min;
                sliders[id]->max = max;
            }
        }

        void InitMultiSlider(const json& val) {
            if (!val.contains("id") || !val["id"].is_string()) {
                // throw?
            }

            auto id = val["id"].template get<std::string>();
            auto numValues = val.contains("numValues") && val["numValues"].is_number() ? val["numValues"].template get<int>() : 2;
            
            auto min = val.contains("min") && val["min"].is_number() ? val["min"].template get<float>() : 0.0f;
            auto max = val.contains("max") && val["max"].is_number() ? val["max"].template get<float>() : 10.0f;
            auto label = val.contains("label") && val["label"].is_string() ? val["label"].template get<std::string>() : "";

            if (multiSliders.contains(id)) {
                multiSliders[id]->label = label;
                
                // todo: Changing numValues should probably throw - or emit a warning?

                multiSliders[id]->min = min;
                multiSliders[id]->max = max;
            } else {
                multiSliders[id] = std::make_unique<MultiSliderStuff>();

                multiSliders[id]->label = label;
                multiSliders[id]->numValues = numValues;
                multiSliders[id]->values = std::make_unique<float[]>(numValues);
                multiSliders[id]->min = min;
                multiSliders[id]->max = max;

                if (val.contains("defaultValues") && val["defaultValues"].is_array()) {
                    for (auto& [key, item] : val["defaultValues"].items()) {
                        multiSliders[id]->values[stoi(key)] = item.template get<float>();
                    }
                }
            }
        }

        void InitInputText(const json& val) {
            if (!val.contains("id") || !val["id"].is_string()) {
                // throw?
            }

            auto id = val["id"].template get<std::string>();
            auto defaultValue = val.contains("defaultValue") && val["defaultValue"].is_string() ? val["defaultValue"].template get<std::string>() : "";
            auto label = val.contains("label") && val["label"].is_string() ? val["label"].template get<std::string>() : "";

            if (inputTexts.contains(id)) {
                inputTexts[id]->label = label;
            } else {
                inputTexts[id] = std::make_unique<InputTextStuff>();

                inputTexts[id]->bufferPointer = std::make_unique<char[]>(100);
                inputTexts[id]->onInputTextChange = onInputTextChange;
                inputTexts[id]->widgetId = id;
                inputTexts[id]->defaultValue = defaultValue;
                inputTexts[id]->label = label;

                strncpy(inputTexts[id].get()->bufferPointer.get(), defaultValue.c_str(), 99);
            }
        }

        void InitCombo(const json& val) {
            if (!val.contains("id") || !val["id"].is_string()) {
                // throw?
            }

            auto id = val["id"].template get<std::string>();
            auto defaultValue = val.contains("defaultValue") && val["defaultValue"].is_number() ? val["defaultValue"].template get<int>() : 0;
            auto label = val["label"].template get<std::string>();
            
            if (combos.contains(id)) {
                combos[id]->label = label;
            } else {
                combos[id] = Combo::getComboStuff(label, defaultValue, val["optionsList"].template get<std::string>());
            }
        }

    public:
        ReactImgui(
            emscripten::val onInputTextChangeFn,
            emscripten::val onComboChangeFn,
            emscripten::val onNumericValueChangeFn,
            emscripten::val onMultiValueChange,
            emscripten::val onBooleanValueChangeFn,
            emscripten::val onClickFn,
            const char* newWindowId, 
            const char* newGlWindowTitle
        ) : ImPlotView(newWindowId, newGlWindowTitle) {
            onInputTextChange = std::make_unique<emscripten::val>(onInputTextChangeFn);
            onComboChange = std::make_unique<emscripten::val>(onComboChangeFn);
            onNumericValueChange = std::make_unique<emscripten::val>(onNumericValueChangeFn);
            onBooleanValueChange = std::make_unique<emscripten::val>(onBooleanValueChangeFn);
            onClick = std::make_unique<emscripten::val>(onClickFn);
        }

        void PrepareForRender() {
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
        }

        void Render(int window_width, int window_height) {
            SetCurrentContext();

            // ImGuiIO& io = ImGui::GetIO();

            ImGui_ImplWGPU_NewFrame();
            ImGui_ImplGlfw_NewFrame();

            ImGui::NewFrame();

            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(window_width, window_height));

            ImGui::Begin(windowId, NULL, window_flags);

            RenderWidgets(widgets);

            ImGui::End();
            ImGui::Render();
        }

        void SetWidgets(std::string widgetsJson) {
            widgets = json::parse(widgetsJson);

            // printf("%s\n", widgetsJson.c_str());

            InitWidgets(widgets);
            
        }

        json GetAvailableFonts() {
            SetCurrentContext();
            ImGuiIO& io = ImGui::GetIO();
            json fonts = json::array();

            // printf("Test!\n");

            for (ImFont* font : io.Fonts->Fonts) {
                fonts.push_back(font->GetDebugName());

                printf("%s\n", font->GetDebugName());
            }

            return fonts;
        }
};



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

struct MultiSliderStuff {
    std::unique_ptr<float[]> values;
    float min;
    float max;
    int numValues;
    int decimalDigits;
    std::string label;
};

struct TabItemStuff {
    bool visible; // ImGui misleadingly refers to this as `p_open`
    std::string label;
};

struct ButtonStuff {
    std::string label;
};

class Widget {

};

class Combo final : public Widget {
    protected:
        Combo(std::string label, int defaultValue, const json& options) {
            this->selectedIndex = defaultValue;
            this->label = label;
            this->itemsSeparatedByZeros = Combo::getItemsSeparatedByZeros(options);
        }
        
        Combo(std::string label, int defaultValue, std::string optionsList) {
            this->selectedIndex = defaultValue;
            this->label = label;
            this->itemsSeparatedByZeros = Combo::getItemsSeparatedByZeros(optionsList);
        }

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
        int selectedIndex;
        std::string label;
        std::unique_ptr<char[]> itemsSeparatedByZeros; // Relevant for 'basic' combo only

        static std::unique_ptr<Combo> makeComboWidget(std::string label, int defaultValue, const json& options) {
            Combo instance(label, defaultValue, options);
            return std::make_unique<Combo>(std::move(instance));
        }

        static std::unique_ptr<Combo> makeComboWidget(std::string label, int defaultValue, std::string optionsList) {
            Combo instance(label, defaultValue, optionsList);
            return std::make_unique<Combo>(std::move(instance));
        }
};

class InputText final : public Widget {
    protected:
        InputText(std::string id, std::string defaultValue, std::string label) {
            this->bufferPointer = std::make_unique<char[]>(100);
            this->widgetId = id;
            this->defaultValue = defaultValue;
            this->label = label;

            strncpy(this->bufferPointer.get(), defaultValue.c_str(), 99);
        }

    public:
        std::unique_ptr<char[]> bufferPointer;
        std::string widgetId;
        std::string defaultValue;
        std::string label;

        inline static emscripten::val onInputTextChange_;

        static std::unique_ptr<InputText> makeInputTextWidget(std::string id, std::string defaultValue, std::string label) {
            InputText instance(id, defaultValue, label);
            return std::make_unique<InputText>(std::move(instance));
        }
};

class Checkbox final : public Widget {
    protected:
        Checkbox(std::string label, bool defaultChecked) {
            this->checked = defaultChecked;
            this->label = label;
        }

    public:
        bool checked;
        std::string label;

        static std::unique_ptr<Checkbox> makeCheckboxWidget(std::string label, bool defaultChecked) {
            Checkbox instance(label, defaultChecked);
            return std::make_unique<Checkbox>(std::move(instance));
        }
};

class Slider final : public Widget {
    protected:
        Slider(std::string label, float defaultValue, float min, float max, std::string sliderType) {
            this->type = sliderType;
            this->label = label;
            this->value = defaultValue;
            this->min = min;
            this->max = max;
        }

    public:
        std::string type;
        float value;
        float min;
        float max;
        std::string label;

        static std::unique_ptr<Slider> makeSliderWidget(std::string label, float defaultValue, float min, float max, std::string sliderType) {
            Slider instance(label, defaultValue, min, max, sliderType);
            return std::make_unique<Slider>(std::move(instance));
        }
};

class ReactImgui final : public ImPlotView {
    typedef std::function<void(const json&)> rendererFunction;

    private:
        json widgets;
        std::unordered_map<std::string, std::unique_ptr<InputText>> inputTexts;
        std::unordered_map<std::string, std::unique_ptr<Combo>> combos;
        std::unordered_map<std::string, std::unique_ptr<Slider>> sliders;
        std::unordered_map<std::string, std::unique_ptr<MultiSliderStuff>> multiSliders;
        std::unordered_map<std::string, std::unique_ptr<Checkbox>> checkboxes;
        std::unordered_map<std::string, std::unique_ptr<ButtonStuff>> buttons;
        std::unordered_map<std::string, std::unique_ptr<TabItemStuff>> tabItems;

        std::unordered_map<std::string, rendererFunction> rendererFunctionMap;
        std::unordered_map<int, std::unique_ptr<char[]>> floatFormatChars;

        std::shared_ptr<emscripten::val> onInputTextChange;
        std::unique_ptr<emscripten::val> onComboChange;
        std::unique_ptr<emscripten::val> onNumericValueChange;
        std::unique_ptr<emscripten::val> onMultiValueChange;
        std::unique_ptr<emscripten::val> onBooleanValueChange;
        std::unique_ptr<emscripten::val> onClick;

        ImGuiInputTextFlags inputTextFlags = ImGuiInputTextFlags_CallbackEdit | ImGuiInputTextFlags_NoUndoRedo;

        template <typename T> 
        static emscripten::val ConvertArrayPointerToJsArray(T* arr, int size) {
            const T *end = &arr[size];

            emscripten::val jsArray = emscripten::val::array();

            for (T * curr = arr; curr != end; ++curr) {
                jsArray.call<void>("push", *curr);
            }

            return jsArray;
        }

        static int InputTextCb(ImGuiInputTextCallbackData* data)
        {
            if (data->EventFlag == ImGuiInputTextFlags_CallbackEdit) {
                auto inputText = reinterpret_cast<InputText*>(data->UserData);

                std::string value = data->Buf;
                InputText::onInputTextChange_.call<void>("call", 0, inputText->widgetId, value);
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

                    if (rendererFunctionMap.contains(type)) {
                        rendererFunctionMap[type](val);
                    }
                }
            }
        }

        void RenderSeparator(const json& val) {
            ImGui::Separator();
        }

        void RenderSameLine(const json& val) {
            ImGui::SameLine();
        }

        void RenderUnindent(const json& val) {
            ImGui::Unindent();
        }

        void RenderIndent(const json& val) {
            ImGui::Indent();
        }

        void RenderSeparatorText(const json& val) {
            ImGui::SeparatorText(val["label"].template get<std::string>().c_str());
        }

        void RenderBulletText(const json& val) {
            ImGui::BulletText(val["text"].template get<std::string>().c_str());
        }

        void RenderUnformattedText(const json& val) {
            ImGui::TextUnformatted(val["text"].template get<std::string>().c_str());
        }

        void RenderTextDisabled(const json& val) {
            ImGui::TextDisabled(val["text"].template get<std::string>().c_str());
        }

        void RenderCollapsingHeader(const json& val) {
            ImGui::PushID(val["id"].template get<std::string>().c_str());
            if (ImGui::CollapsingHeader(val["label"].template get<std::string>().c_str())) {
                if (val.contains("children")) {
                    RenderWidgets(val["children"]);
                }
            }
            ImGui::PopID();
        }

        void RenderTextWrap(const json& val) {
            ImGui::PushTextWrapPos(val["width"].template get<double>());
            if (val.contains("children")) {
                RenderWidgets(val["children"]);
            }
            ImGui::PopTextWrapPos();
        }

        void RenderItemTooltip(const json& val) {
            if (ImGui::BeginItemTooltip()) {
                if (val.contains("children")) {
                    RenderWidgets(val["children"]);
                }

                ImGui::EndTooltip();
            }
        }

        void RenderTreeNode(const json& val) {
            ImGui::PushID(val["id"].template get<std::string>().c_str());
            if (ImGui::TreeNode(val["label"].template get<std::string>().c_str())) {
                if (val.contains("children")) {
                    RenderWidgets(val["children"]);
                }

                ImGui::TreePop();
                ImGui::Spacing();
            }
            ImGui::PopID();
        }

        void RenderTabBar(const json& val) {
            ImGui::PushID(val["id"].template get<std::string>().c_str());
            if (ImGui::BeginTabBar(val["id"].template get<std::string>().c_str(), ImGuiTabBarFlags_None)) {
                if (val.contains("children")) {
                    RenderWidgets(val["children"]);
                }
                ImGui::EndTabBar();
            }
            ImGui::PopID();
        }

        void RenderFragment(const json& val) {
            if (val.contains("children")) {
                RenderWidgets(val["children"]);
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
                if (sliders[id]->type == "angle") {
                    if (ImGui::SliderAngle(sliders[id].get()->label.c_str(), &sliders[id]->value, sliders[id]->min, sliders[id]->max, "%.0f")) { // min and max are not passed by reference, it's a copy every time...
                        onNumericValueChange->call<void>("call", 0, id, sliders[id]->value);
                    }
                } else {
                    if (ImGui::SliderFloat(sliders[id].get()->label.c_str(), &sliders[id]->value, sliders[id]->min, sliders[id]->max, "%.0f")) { // min and max are not passed by reference, it's a copy every time...
                        onNumericValueChange->call<void>("call", 0, id, sliders[id]->value);
                    }
                }
                ImGui::PopID();
            }
        }

        void RenderMultiSlider(const json& val) {
            auto id = val["id"].template get<std::string>();
            const char* idAsChar = id.c_str();

            if (multiSliders.contains(id)) {
                ImGui::PushID(idAsChar);

                if (multiSliders[id]->numValues == 2) {
                    if (ImGui::SliderFloat2(multiSliders[id].get()->label.c_str(), multiSliders[id]->values.get(), multiSliders[id]->min, multiSliders[id]->max, floatFormatChars[multiSliders[id]->decimalDigits].get())) {
                        onMultiValueChange->call<void>("call", 0, id, ReactImgui::ConvertArrayPointerToJsArray(multiSliders[id]->values.get(), multiSliders[id]->numValues));
                    }
                } else if (multiSliders[id]->numValues == 3) {
                    if (ImGui::SliderFloat3(multiSliders[id].get()->label.c_str(), multiSliders[id]->values.get(), multiSliders[id]->min, multiSliders[id]->max, floatFormatChars[multiSliders[id]->decimalDigits].get())) {
                        onMultiValueChange->call<void>("call", 0, id, ReactImgui::ConvertArrayPointerToJsArray(multiSliders[id]->values.get(), multiSliders[id]->numValues));
                    }
                } else if (multiSliders[id]->numValues == 4) {
                    if (ImGui::SliderFloat4(multiSliders[id].get()->label.c_str(), multiSliders[id]->values.get(), multiSliders[id]->min, multiSliders[id]->max, floatFormatChars[multiSliders[id]->decimalDigits].get())) {
                        onMultiValueChange->call<void>("call", 0, id, ReactImgui::ConvertArrayPointerToJsArray(multiSliders[id]->values.get(), multiSliders[id]->numValues));
                    }
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
                        InitBasicCombo(val);
                    } else if (type == "Slider") {
                        InitSlider(val);
                    } else if (type == "MultiSlider") {
                        InitMultiSlider(val);
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
                checkboxes[id] = Checkbox::makeCheckboxWidget(label, defaultChecked);
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
            auto sliderType = val.contains("sliderType") && val["sliderType"].is_string() ? val["sliderType"].template get<std::string>() : "default";

            if (sliders.contains(id)) {
                sliders[id]->label = label;
                
                if (sliders[id]->value > max) {
                    // Don't want to go out of bounds!
                    sliders[id]->value = max;
                }

                sliders[id]->min = min;
                sliders[id]->max = max;
            } else {
                sliders[id] = Slider::makeSliderWidget(label, defaultValue, min, max, sliderType);
            }
        }

        void InitMultiSlider(const json& val) {
            if (!val.contains("id") || !val["id"].is_string()) {
                // throw?
            }

            auto id = val["id"].template get<std::string>();
            auto numValues = val.contains("numValues") && val["numValues"].is_number() ? val["numValues"].template get<int>() : 2;
            auto decimalDigits = val.contains("decimalDigits") && val["decimalDigits"].is_number() ? val["decimalDigits"].template get<int>() : 0;
            
            auto min = val.contains("min") && val["min"].is_number() ? val["min"].template get<float>() : 0.0f;
            auto max = val.contains("max") && val["max"].is_number() ? val["max"].template get<float>() : 10.0f;
            auto label = val.contains("label") && val["label"].is_string() ? val["label"].template get<std::string>() : "";

            if (multiSliders.contains(id)) {
                multiSliders[id]->label = label;
                
                // todo: Changing numValues should probably throw - or emit a warning?

                multiSliders[id]->min = min;
                multiSliders[id]->max = max;
                multiSliders[id]->decimalDigits = decimalDigits;
            } else {
                multiSliders[id] = std::make_unique<MultiSliderStuff>();

                multiSliders[id]->label = label;
                multiSliders[id]->numValues = numValues;
                multiSliders[id]->values = std::make_unique<float[]>(numValues);
                multiSliders[id]->min = min;
                multiSliders[id]->max = max;
                multiSliders[id]->decimalDigits = decimalDigits;

                if (val.contains("defaultValues") && val["defaultValues"].is_array() && val["defaultValues"].size() == numValues) {
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
                inputTexts[id] = InputText::makeInputTextWidget(id, defaultValue, label);
            }
        }

        void InitBasicCombo(const json& val) {
            if (!val.contains("id") || !val["id"].is_string()) {
                // throw?
            }

            auto id = val["id"].template get<std::string>();
            auto defaultValue = val.contains("defaultValue") && val["defaultValue"].is_number() ? val["defaultValue"].template get<int>() : 0;
            auto label = val["label"].template get<std::string>();
            
            if (combos.contains(id)) {
                combos[id]->label = label;
            } else {
                combos[id] = Combo::makeComboWidget(label, defaultValue, val["optionsList"].template get<std::string>());
            }
        }

    public:
        ReactImgui(
            emscripten::val onInputTextChangeFn,
            emscripten::val onComboChangeFn,
            emscripten::val onNumericValueChangeFn,
            emscripten::val onMultiValueChangeFn,
            emscripten::val onBooleanValueChangeFn,
            emscripten::val onClickFn,
            const char* newWindowId, 
            const char* newGlWindowTitle
        ) : ImPlotView(newWindowId, newGlWindowTitle) {
            onInputTextChange = std::make_unique<emscripten::val>(onInputTextChangeFn);
            onComboChange = std::make_unique<emscripten::val>(onComboChangeFn);
            onNumericValueChange = std::make_unique<emscripten::val>(onNumericValueChangeFn);
            onMultiValueChange = std::make_unique<emscripten::val>(onMultiValueChangeFn);
            onBooleanValueChange = std::make_unique<emscripten::val>(onBooleanValueChangeFn);
            onClick = std::make_unique<emscripten::val>(onClickFn);

            InputText::onInputTextChange_ = onInputTextChangeFn;

            rendererFunctionMap["Button"] = std::bind(&ReactImgui::RenderButton, this, std::placeholders::_1);
            rendererFunctionMap["Checkbox"] = std::bind(&ReactImgui::RenderCheckbox, this, std::placeholders::_1);
            rendererFunctionMap["MultiSlider"] = std::bind(&ReactImgui::RenderMultiSlider, this, std::placeholders::_1);
            rendererFunctionMap["Slider"] = std::bind(&ReactImgui::RenderSlider, this, std::placeholders::_1);
            rendererFunctionMap["Combo"] = std::bind(&ReactImgui::RenderBasicCombo, this, std::placeholders::_1);
            rendererFunctionMap["InputText"] = std::bind(&ReactImgui::RenderInputText, this, std::placeholders::_1);
            rendererFunctionMap["Fragment"] = std::bind(&ReactImgui::RenderFragment, this, std::placeholders::_1);
            rendererFunctionMap["TabBar"] = std::bind(&ReactImgui::RenderTabBar, this, std::placeholders::_1);
            rendererFunctionMap["TabItem"] = std::bind(&ReactImgui::RenderTabItem, this, std::placeholders::_1);
            rendererFunctionMap["TreeNode"] = std::bind(&ReactImgui::RenderTreeNode, this, std::placeholders::_1);
            rendererFunctionMap["ItemTooltip"] = std::bind(&ReactImgui::RenderItemTooltip, this, std::placeholders::_1);
            rendererFunctionMap["TextWrap"] = std::bind(&ReactImgui::RenderTextWrap, this, std::placeholders::_1);
            rendererFunctionMap["CollapsingHeader"] = std::bind(&ReactImgui::RenderCollapsingHeader, this, std::placeholders::_1);
            rendererFunctionMap["TextDisabled"] = std::bind(&ReactImgui::RenderTextDisabled, this, std::placeholders::_1);
            rendererFunctionMap["UnformattedText"] = std::bind(&ReactImgui::RenderUnformattedText, this, std::placeholders::_1);
            rendererFunctionMap["BulletText"] = std::bind(&ReactImgui::RenderBulletText, this, std::placeholders::_1);
            rendererFunctionMap["SeparatorText"] = std::bind(&ReactImgui::RenderSeparatorText, this, std::placeholders::_1);
            rendererFunctionMap["Indent"] = std::bind(&ReactImgui::RenderIndent, this, std::placeholders::_1);
            rendererFunctionMap["Unindent"] = std::bind(&ReactImgui::RenderUnindent, this, std::placeholders::_1);
            rendererFunctionMap["SameLine"] = std::bind(&ReactImgui::RenderSameLine, this, std::placeholders::_1);
            rendererFunctionMap["Separator"] = std::bind(&ReactImgui::RenderSeparator, this, std::placeholders::_1);

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



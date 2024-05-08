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

using json = nlohmann::json;

#pragma once

class ReactImgui;

class Widget {
    public:
        int id;

        inline static emscripten::val onInputTextChange_;

        Widget(int id) {
            this->id = id;
        }

        void HandleChildren(ReactImgui* view);

        virtual void Render(ReactImgui* view) = 0;
};

class Fragment final : public Widget {
    public:
        std::string label;

        Fragment(int id) : Widget(id) {}

        void Render(ReactImgui* view);
};

class SameLine final : public Widget {
    public:
        SameLine(int id) : Widget(id) {}

        void Render(ReactImgui* view);
};

class Separator final : public Widget {
    public:
        Separator(int id) : Widget(id) {}

        void Render(ReactImgui* view);
};

class Indent final : public Widget {
    public:
        Indent(int id) : Widget(id) {}

        void Render(ReactImgui* view);
};

class Unindent final : public Widget {
    public:
        Unindent(int id) : Widget(id) {}

        void Render(ReactImgui* view);
};

class SeparatorText final : public Widget {
    public:
        std::string label;

        SeparatorText(int id, std::string label) : Widget(id) {
            this->label = label;
        }

        void Render(ReactImgui* view);
};

class BulletText final : public Widget {
    public:
        std::string text;

        BulletText(int id, std::string text) : Widget(id) {
            this->text = text;
        }

        void Render(ReactImgui* view);
};

class UnformattedText final : public Widget {
    public:
        std::string text;

        UnformattedText(int id, std::string text) : Widget(id) {
            this->text = text;
        }

        void Render(ReactImgui* view);
};

class DisabledText final : public Widget {
    public:
        std::string text;

        DisabledText(int id, std::string text) : Widget(id) {
            this->text = text;
        }

        void Render(ReactImgui* view);
};

class TabBar final : public Widget {
    public:
        std::string label;

        TabBar(int id, std::string label) : Widget(id) {
            this->label = label;
        }

        void Render(ReactImgui* view);
};

class TabItem final : public Widget {
    public:
        std::string label;

        TabItem(int id, std::string label) : Widget(id) {
            this->label = label;
        }

        void Render(ReactImgui* view);
};

class CollapsingHeader final : public Widget {
    public:
        std::string label;

        CollapsingHeader(int id, std::string label) : Widget(id) {
            this->label = label;
        }

        void Render(ReactImgui* view);
};

class TextWrap final : public Widget {
    public:
        double width;

        TextWrap(int id, double width) : Widget(id) {
            this->width = width;
        }

        void Render(ReactImgui* view);
};

class ItemTooltip final : public Widget {
    public:
        ItemTooltip(int id) : Widget(id) {}

        void Render(ReactImgui* view);
};

class TreeNode final : public Widget {
    public:
        std::string label;

        TreeNode(int id, std::string label) : Widget(id) {
            this->label = label;
        }

        void Render(ReactImgui* view);
};

class Combo final : public Widget {
    protected:
        Combo(int id, std::string label, int defaultValue, const json& options) : Widget(id) {
            this->selectedIndex = defaultValue;
            this->label = label;
            this->itemsSeparatedByZeros = Combo::getItemsSeparatedByZeros(options);
        }
        
        Combo(int id, std::string label, int defaultValue, std::string optionsList) : Widget(id) {
            this->selectedIndex = defaultValue;
            this->label = label;
            this->itemsSeparatedByZeros = Combo::getItemsSeparatedByZeros(optionsList);
        }

        /**
         * Takes array of [{value: number; label: string}] and converts into "label\0label\0label\0" (double NULL character at the end)
         * because this is what ImGui:Combo() expects
        */
        inline static std::unique_ptr<char[]> getItemsSeparatedByZeros(const json& options) {
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
        inline static std::unique_ptr<char[]> getItemsSeparatedByZeros(std::string optionsList) {
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

        static std::unique_ptr<Combo> makeComboWidget(int id, std::string label, int defaultValue, const json& options) {
            Combo instance(id, label, defaultValue, options);
            return std::make_unique<Combo>(std::move(instance));
        }

        static std::unique_ptr<Combo> makeComboWidget(int id, std::string label, int defaultValue, std::string optionsList) {
            Combo instance(id, label, defaultValue, optionsList);
            return std::make_unique<Combo>(std::move(instance));
        }

        void Render(ReactImgui* view);
};

class InputText final : public Widget {
    protected:
        inline static ImGuiInputTextFlags inputTextFlags = ImGuiInputTextFlags_CallbackEdit | ImGuiInputTextFlags_NoUndoRedo;

        static int InputTextCb(ImGuiInputTextCallbackData* data)
        {
            if (data->EventFlag == ImGuiInputTextFlags_CallbackEdit) {
                auto pInputText = reinterpret_cast<InputText*>(data->UserData);

                std::string value = data->Buf;
                Widget::onInputTextChange_.call<void>("call", 0, pInputText->id, value);
            }

            return 0;
        }

        InputText(int id, std::string defaultValue, std::string label) : Widget(id) {
            this->bufferPointer = std::make_unique<char[]>(100);
            this->defaultValue = defaultValue;
            this->label = label;

            strncpy(this->bufferPointer.get(), defaultValue.c_str(), 99);
        }

    public:
        std::unique_ptr<char[]> bufferPointer;
        std::string defaultValue;
        std::string label;

        inline static std::unique_ptr<InputText> makeInputTextWidget(int id, std::string defaultValue, std::string label) {
            InputText instance(id, defaultValue, label);
            return std::make_unique<InputText>(std::move(instance));
        }

        void Render(ReactImgui* view);
};

class Checkbox final : public Widget {
    protected:
        Checkbox(int id, std::string label, bool defaultChecked) : Widget(id) {
            this->checked = defaultChecked;
            this->label = label;
        }

    public:
        bool checked;
        std::string label;

        inline static std::unique_ptr<Checkbox> makeCheckboxWidget(int id, std::string label, bool defaultChecked) {
            Checkbox instance(id, label, defaultChecked);
            return std::make_unique<Checkbox>(std::move(instance));
        }

        void Render(ReactImgui* view);
};

class Button final : public Widget {
    protected:
        Button(int id, std::string label) : Widget(id) {
            this->label = label;
        }

    public:
        std::string label;

        inline static std::unique_ptr<Button> makeButtonWidget(int id, std::string label) {
            Button instance(id, label);
            return std::make_unique<Button>(std::move(instance));
        }

        void Render(ReactImgui* view);
};

class Slider final : public Widget {
    protected:
        Slider(int id, std::string label, float defaultValue, float min, float max, std::string sliderType) : Widget(id) {
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

        inline static std::unique_ptr<Slider> makeSliderWidget(int id, std::string label, float defaultValue, float min, float max, std::string sliderType) {
            Slider instance(id, label, defaultValue, min, max, sliderType);
            return std::make_unique<Slider>(std::move(instance));
        }

        void Render(ReactImgui* view);
};

class MultiSlider final : public Widget {
    protected:
        MultiSlider(int id, std::string label, float min, float max, int numValues, int decimalDigits) : Widget(id) {
            this->label = label;
            this->numValues = numValues;
            this->values = std::make_unique<float[]>(numValues);
            this->min = min;
            this->max = max;
            this->decimalDigits = decimalDigits;
        }

    public:
        std::unique_ptr<float[]> values;
        float min;
        float max;
        int numValues;
        int decimalDigits;
        std::string label;

        static std::unique_ptr<MultiSlider> makeMultiSliderWidget(int id, std::string label, float min, float max, int numValues, int decimalDigits, const json& defaultValues) {
            MultiSlider instance(id, label, min, max, numValues, decimalDigits);

            for (auto& [key, item] : defaultValues.items()) {
                instance.values[stoi(key)] = item.template get<float>();
            }

            return std::make_unique<MultiSlider>(std::move(instance));
        }

        static std::unique_ptr<MultiSlider> makeMultiSliderWidget(int id, std::string label, float min, float max, int numValues, int decimalDigits) {
            MultiSlider instance(id, label, min, max, numValues, decimalDigits);

            return std::make_unique<MultiSlider>(std::move(instance));
        }

        void Render(ReactImgui* view);
};


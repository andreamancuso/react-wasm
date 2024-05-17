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

#include "shared.h"

using json = nlohmann::json;

#pragma once

class ReactImgui;

class Widget {
    public:
        int m_id;
        std::string m_type;
        bool m_handlesChildrenWithinRenderMethod;

        inline static OnTextChangedCallback onInputTextChange_;

        Widget(int id) {
            m_id = id;
            m_type = "Unknown";
            m_handlesChildrenWithinRenderMethod = false;
        }

        void HandleChildren(ReactImgui* view);

        virtual void Render(ReactImgui* view) = 0;
};

class Fragment final : public Widget {
    public:
        std::string m_label;

        Fragment(int id) : Widget(id) {
            m_type = "Fragment";
            m_handlesChildrenWithinRenderMethod = true;
        }

        void Render(ReactImgui* view);
};

class SameLine final : public Widget {
    public:
        SameLine(int id) : Widget(id) {
            m_type = "SameLine";
            m_handlesChildrenWithinRenderMethod = true;
        }

        void Render(ReactImgui* view);
};

class Separator final : public Widget {
    public:
        Separator(int id) : Widget(id) {}

        void Render(ReactImgui* view);
};

class Indent final : public Widget {
    public:
        Indent(int id) : Widget(id) {
            m_type = "Indent";
            m_handlesChildrenWithinRenderMethod = true;
        }

        void Render(ReactImgui* view);
};

// Likely unused
class Unindent final : public Widget {
    public:
        Unindent(int id) : Widget(id) {
            m_type = "Unindent";
        }

        void Render(ReactImgui* view);
};

class SeparatorText final : public Widget {
    public:
        std::string m_label;

        SeparatorText(int id, std::string label) : Widget(id) {
            m_type = "SeparatorText";
            m_label = label;
        }

        void Render(ReactImgui* view);
};

class BulletText final : public Widget {
    public:
        std::string m_text;

        BulletText(int id, std::string text) : Widget(id) {
            m_type = "BulletText";
            m_text = text;
        }

        void Render(ReactImgui* view);
};

class UnformattedText final : public Widget {
    public:
        std::string m_text;

        UnformattedText(int id, std::string text) : Widget(id) {
            m_type = "UnformattedText";
            m_text = text;
        }

        void Render(ReactImgui* view);
};

class DisabledText final : public Widget {
    public:
        std::string m_text;

        DisabledText(int id, std::string text) : Widget(id) {
            m_type = "DisabledText";
            m_text = text;
        }

        void Render(ReactImgui* view);
};

class TabBar final : public Widget {
    public:
        TabBar(int id) : Widget(id) {
            m_type = "TabBar";
            m_handlesChildrenWithinRenderMethod = true;
        }

        void Render(ReactImgui* view);
};

class TabItem final : public Widget {
    public:
        std::string m_label;

        TabItem(int id, std::string label) : Widget(id) {
            m_type = "TabItem";
            m_handlesChildrenWithinRenderMethod = true;
            m_label = label;
        }

        void Render(ReactImgui* view);
};

class CollapsingHeader final : public Widget {
    public:
        std::string m_label;

        CollapsingHeader(int id, std::string label) : Widget(id) {
            m_type = "CollapsingHeader";
            m_handlesChildrenWithinRenderMethod = true;
            m_label = label;
        }

        void Render(ReactImgui* view);
};

class TextWrap final : public Widget {
    public:
        double m_width;

        TextWrap(int id, double width) : Widget(id) {
            m_type = "TextWrap";
            m_handlesChildrenWithinRenderMethod = true;
            m_width = width;
        }

        void Render(ReactImgui* view);
};

class ItemTooltip final : public Widget {
    public:
        ItemTooltip(int id) : Widget(id) {
            m_type = "ItemTooltip";
            m_handlesChildrenWithinRenderMethod = true;
        }

        void Render(ReactImgui* view);
};

class TreeNode final : public Widget {
    public:
        std::string m_label;

        TreeNode(int id, std::string label) : Widget(id) {
            m_type = "TreeNode";
            m_handlesChildrenWithinRenderMethod = true;
            m_label = label;
        }

        void Render(ReactImgui* view);
};

class Combo final : public Widget {
    protected:
        Combo(int id, std::string label, int defaultValue, const json& options) : Widget(id) {
            m_type = "Combo";
            m_selectedIndex = defaultValue;
            m_label = label;
            m_itemsSeparatedByZeros = Combo::getItemsSeparatedByZeros(options);
        }
        
        Combo(int id, std::string label, int defaultValue, std::string optionsList) : Widget(id) {
            m_type = "Combo";
            m_selectedIndex = defaultValue;
            m_label = label;
            m_itemsSeparatedByZeros = Combo::getItemsSeparatedByZeros(optionsList);
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
        int m_selectedIndex;
        std::string m_label;
        std::unique_ptr<char[]> m_itemsSeparatedByZeros; // Relevant for 'basic' combo only

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

        static int InputTextCb(ImGuiInputTextCallbackData* data);

        InputText(int id, std::string defaultValue, std::string label) : Widget(id) {
            m_type = "InputText";
            m_bufferPointer = std::make_unique<char[]>(100);
            m_defaultValue = defaultValue;
            m_label = label;

            strncpy(m_bufferPointer.get(), defaultValue.c_str(), 99);
        }

    public:
        std::unique_ptr<char[]> m_bufferPointer;
        std::string m_defaultValue;
        std::string m_label;

        inline static std::unique_ptr<InputText> makeInputTextWidget(int id, std::string defaultValue, std::string label) {
            InputText instance(id, defaultValue, label);
            return std::make_unique<InputText>(std::move(instance));
        }

        void Render(ReactImgui* view);
};

class Checkbox final : public Widget {
    protected:
        Checkbox(int id, std::string label, bool defaultChecked) : Widget(id) {
            m_type = "Checkbox";
            m_checked = defaultChecked;
            m_label = label;
        }

    public:
        bool m_checked;
        std::string m_label;

        inline static std::unique_ptr<Checkbox> makeCheckboxWidget(int id, std::string label, bool defaultChecked) {
            Checkbox instance(id, label, defaultChecked);
            return std::make_unique<Checkbox>(std::move(instance));
        }

        void Render(ReactImgui* view);
};

class Button final : public Widget {
    protected:
        Button(int id, std::string label) : Widget(id) {
            m_type = "Button";
            m_label = label;
        }

    public:
        std::string m_label;

        inline static std::unique_ptr<Button> makeButtonWidget(int id, std::string label) {
            Button instance(id, label);
            return std::make_unique<Button>(std::move(instance));
        }

        void Render(ReactImgui* view);
};

class Slider final : public Widget {
    protected:
        Slider(int id, std::string label, float defaultValue, float min, float max, std::string sliderType) : Widget(id) {
            m_type = "Slider";
            m_sliderType = sliderType;
            m_label = label;
            m_value = defaultValue;
            m_min = min;
            m_max = max;
        }

    public:
        std::string m_sliderType;
        float m_value;
        float m_min;
        float m_max;
        std::string m_label;

        inline static std::unique_ptr<Slider> makeSliderWidget(int id, std::string label, float defaultValue, float min, float max, std::string sliderType) {
            Slider instance(id, label, defaultValue, min, max, sliderType);
            return std::make_unique<Slider>(std::move(instance));
        }

        void Render(ReactImgui* view);
};

class MultiSlider final : public Widget {
    protected:
        MultiSlider(int id, std::string label, float min, float max, int numValues, int decimalDigits) : Widget(id) {
            m_type = "MultiSlider";
            m_label = label;
            m_numValues = numValues;
            m_values = std::make_unique<float[]>(numValues);
            m_min = min;
            m_max = max;
            m_decimalDigits = decimalDigits;
        }

    public:
        std::unique_ptr<float[]> m_values;
        float m_min;
        float m_max;
        int m_numValues;
        int m_decimalDigits;
        std::string m_label;

        static std::unique_ptr<MultiSlider> makeMultiSliderWidget(int id, std::string label, float min, float max, int numValues, int decimalDigits, const json& defaultValues) {
            MultiSlider instance(id, label, min, max, numValues, decimalDigits);

            for (auto& [key, item] : defaultValues.items()) {
                instance.m_values[stoi(key)] = item.template get<float>();
            }

            return std::make_unique<MultiSlider>(std::move(instance));
        }

        static std::unique_ptr<MultiSlider> makeMultiSliderWidget(int id, std::string label, float min, float max, int numValues, int decimalDigits) {
            MultiSlider instance(id, label, min, max, numValues, decimalDigits);

            return std::make_unique<MultiSlider>(std::move(instance));
        }

        void Render(ReactImgui* view);
};


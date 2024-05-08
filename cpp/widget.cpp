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

#include "widget.h"
#include "reactimgui.h"

using json = nlohmann::json;

void Widget::HandleChildren(ReactImgui& view) {
    if (view.hierarchy.contains(id)) {
        if (view.hierarchy[id].size() > 0) {
            for (auto& [key, childId] : view.hierarchy[id]) {
                view->renderWidgetById(childId);
            }
        }
    }
};

// todo: see if we can get rid of this class, now that SameLine() isn't very smarty anymore
class Fragment final : public Widget {
    public:
        std::string label;

        Fragment(int id) : Widget(id) {}

        void Render(ReactImgui& view) {
            Widget::HandleChildren(view);
        }
};

class SameLine final : public Widget {
    public:
        SameLine(int id) : Widget(id) {}

        // todo: perhaps it is now possible to do the trick?
        void Render(ReactImgui& view) {
            ImGui::SameLine();
        }
};

class Separator final : public Widget {
    public:
        Separator(int id) : Widget(id) {}

        void Render(ReactImgui& view) {
            ImGui::Separator();
        }
};

class Indent final : public Widget {
    public:
        Indent(int id) : Widget(id) {}

        void Render(ReactImgui& view) {
            ImGui::Indent();
        }
};

class Unindent final : public Widget {
    public:
        Unindent(int id) : Widget(id) {}

        void Render(ReactImgui& view) {
            ImGui::Unindent();
        }
};

class SeparatorText final : public Widget {
    public:
        std::string label;

        SeparatorText(int id, std::string label) : Widget(id) {
            this->label = label;
        }

        void Render(ReactImgui& view) {
            ImGui::SeparatorText(label.c_str());
        }
};

class BulletText final : public Widget {
    public:
        std::string text;

        BulletText(int id, std::string text) : Widget(id) {
            this->text = text;
        }

        void Render(ReactImgui& view) {
            ImGui::BulletText(text.c_str());
        }
};

class UnformattedText final : public Widget {
    public:
        std::string text;

        UnformattedText(int id, std::string text) : Widget(id) {
            this->text = text;
        }

        void Render(ReactImgui& view) {
            ImGui::TextUnformatted(text.c_str());
        }
};

class DisabledText final : public Widget {
    public:
        std::string text;

        DisabledText(int id, std::string text) : Widget(id) {
            this->text = text;
        }

        void Render(ReactImgui& view) {
            ImGui::TextDisabled(text.c_str());
        }
};

class TabBar final : public Widget {
    public:
        std::string label;

        TabBar(int id, std::string label) : Widget(id) {
            this->label = label;
        }

        void Render(ReactImgui& view) {
            ImGui::PushID(id);
            // todo: double-check if we need to pass a proper id here?
            if (ImGui::BeginTabBar("", ImGuiTabBarFlags_None)) {
                Widget::HandleChildren(view);
                ImGui::EndTabBar();
            }
            ImGui::PopID();
        }
};

class TabItem final : public Widget {
    public:
        std::string label;

        TabItem(int id, std::string label) : Widget(id) {
            this->label = label;
        }

        void Render(ReactImgui& view) {
            ImGui::PushID(id);
            if (ImGui::BeginTabItem(label.c_str())) {
                Widget::HandleChildren(view);
                ImGui::EndTabItem();
            }
            ImGui::PopID();
        }
};

class CollapsingHeader final : public Widget {
    public:
        std::string label;

        CollapsingHeader(int id, std::string label) : Widget(id) {
            this->label = label;
        }

        void Render(ReactImgui& view) {
            ImGui::PushID(id);
            if (ImGui::CollapsingHeader(label.c_str())) {
                Widget::HandleChildren(view);
            }
            ImGui::PopID();
        }
};

class TextWrap final : public Widget {
    public:
        double width;

        TextWrap(int id, double width) : Widget(id) {
            this->width = width;
        }

        void Render(ReactImgui& view) {
            ImGui::PushTextWrapPos(width);
            
            Widget::HandleChildren(view);

            ImGui::PopTextWrapPos();
        }
};

class ItemTooltip final : public Widget {
    public:
        ItemTooltip(int id) : Widget(id) {}

        void Render(ReactImgui& view, std::vector<int> children) {
            if (ImGui::BeginItemTooltip()) {
                Widget::HandleChildren(view);

                ImGui::EndTooltip();
            }
        }
};

class TreeNode final : public Widget {
    public:
        std::string label;

        TreeNode(int id, std::string label) : Widget(id) {
            this->label = label;
        }

        void Render(ReactImgui& view) {
            ImGui::PushID(id);
            if (ImGui::TreeNode(label.c_str())) {
                Widget::HandleChildren(view);

                ImGui::TreePop();
                ImGui::Spacing();
            }
            ImGui::PopID();
        }
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

        void Render(ReactImgui& view) {
            ImGui::PushID(id);
            if (ImGui::Combo(label.c_str(), &selectedIndex, itemsSeparatedByZeros.get())) {
                view.onComboChange->call<void>("call", 0, id, selectedIndex);
            }
            ImGui::PopID();
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

        void Render(ReactImgui& view) {
            ImGui::PushID(id);
            ImGui::InputText(
                label.c_str(), 
                bufferPointer.get(), 
                100, 
                inputTextFlags, 
                InputTextCb, 
                (void*)this
            );
            ImGui::PopID();
        }
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

        void Render(ReactImgui& view) {
            ImGui::PushID(id);
            if (ImGui::Checkbox(label.c_str(), &checked)) {
                view.onBooleanValueChange->call<void>("call", 0, id, checked);
            }
            ImGui::PopID();
        }
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

        void Render(ReactImgui& view) {
            ImGui::PushID(id);
            if (ImGui::Button(label.c_str())) {
                view.onClick->call<void>("call", 0, id);
            }
            ImGui::PopID();
        }
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

        void Render(ReactImgui& view) {
            ImGui::PushID(id);
            if (type == "angle") {
                if (ImGui::SliderAngle(label.c_str(), &value, min, max, "%.0f")) {
                    view.onNumericValueChange->call<void>("call", 0, id, value);
                }
            } else {
                if (ImGui::SliderFloat(label.c_str(), &value, min, max, "%.0f")) {
                    view.onNumericValueChange->call<void>("call", 0, id, value);
                }
            }
            ImGui::PopID();
        }
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

        void Render(ReactImgui& view) {
            ImGui::PushID(id);

            if (this->numValues == 2) {
                if (ImGui::SliderFloat2(this->label.c_str(), this->values.get(), this->min, this->max, view.floatFormatChars[this->decimalDigits].get())) {
                    view.onMultiValueChange->call<void>("call", 0, id, ReactImgui::ConvertArrayPointerToJsArray(
                            this->values.get(), 
                            this->numValues
                        )
                    );
                }
            } else if (this->numValues == 3) {
                if (ImGui::SliderFloat3(this->label.c_str(), values.get(), this->min, this->max, view.floatFormatChars[this->decimalDigits].get())) {
                    view.onMultiValueChange->call<void>("call", 0, id, ReactImgui::ConvertArrayPointerToJsArray(
                            this->values.get(), 
                            this->numValues
                        )
                    );
                }
            } else if (this->numValues == 4) {
                if (ImGui::SliderFloat4(this->label.c_str(), values.get(), this->min, this->max, view.floatFormatChars[this->decimalDigits].get())) {
                    onMultiValueChange->call<void>("call", 0, id, ReactImgui::ConvertArrayPointerToJsArray(
                            this->values.get(), 
                            this->numValues
                        )
                    );
                }
            }

            ImGui::PopID();
        }
};

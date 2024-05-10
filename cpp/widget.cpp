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

void Widget::HandleChildren(ReactImgui* view) {
    view->RenderChildren(id);
};

void Fragment::Render(ReactImgui* view) {
    Widget::HandleChildren(view);
};


void SameLine::Render(ReactImgui* view) {
    // Special case
    if (view->hierarchy.contains(id)) {
        size_t size = view->hierarchy[id].size() - 1;

        for (int index = 0; index < view->hierarchy[id].size(); ++index) {
            view->RenderWidgets(view->hierarchy[id][index]);

            if (index < (size)) {
                ImGui::SameLine();
            }
        }
    }
};

void Separator::Render(ReactImgui* view) {
    ImGui::Separator();
};

void Indent::Render(ReactImgui* view) {
    ImGui::Indent();
    Widget::HandleChildren(view);
    ImGui::Unindent();
};

void Unindent::Render(ReactImgui* view) {
    ImGui::Unindent();
};

void SeparatorText::Render(ReactImgui* view) {
    ImGui::SeparatorText(label.c_str());
};

void BulletText::Render(ReactImgui* view) {
    ImGui::BulletText(text.c_str());
};

void UnformattedText::Render(ReactImgui* view) {
    ImGui::TextUnformatted(text.c_str());
};

void DisabledText::Render(ReactImgui* view) {
    ImGui::TextDisabled(text.c_str());
};

void TabBar::Render(ReactImgui* view) {
    ImGui::PushID(id);
    // todo: double-check if we need to pass a proper id here?
    if (ImGui::BeginTabBar("", ImGuiTabBarFlags_None)) {
        Widget::HandleChildren(view);
        ImGui::EndTabBar();
    }
    ImGui::PopID();
};

void TabItem::Render(ReactImgui* view) {
    ImGui::PushID(id);
    if (ImGui::BeginTabItem(label.c_str())) {
        Widget::HandleChildren(view);
        ImGui::EndTabItem();
    }
    ImGui::PopID();
};

void CollapsingHeader::Render(ReactImgui* view) {
    ImGui::PushID(id);
    if (ImGui::CollapsingHeader(label.c_str())) {
        Widget::HandleChildren(view);
    }
    ImGui::PopID();
};

void TextWrap::Render(ReactImgui* view) {
    ImGui::PushTextWrapPos(width);
    
    Widget::HandleChildren(view);

    ImGui::PopTextWrapPos();
};

void ItemTooltip::Render(ReactImgui* view) {
    if (ImGui::BeginItemTooltip()) {
        Widget::HandleChildren(view);

        ImGui::EndTooltip();
    }
};

void TreeNode::Render(ReactImgui* view) {
    ImGui::PushID(id);
    if (ImGui::TreeNode(label.c_str())) {
        Widget::HandleChildren(view);

        ImGui::TreePop();
        ImGui::Spacing();
    }
    ImGui::PopID();
};

void Combo::Render(ReactImgui* view) {
    ImGui::PushID(id);
    if (ImGui::Combo(label.c_str(), &selectedIndex, itemsSeparatedByZeros.get())) {
        view->onComboChange(id, selectedIndex);
    }
    ImGui::PopID();
};

void InputText::Render(ReactImgui* view) {
    ImGui::PushID(id);
    ImGui::InputText(label.c_str(), bufferPointer.get(), 100, inputTextFlags, InputTextCb, (void*)this);
    ImGui::PopID();
};

int InputText::InputTextCb(ImGuiInputTextCallbackData* data)
{
    if (data->EventFlag == ImGuiInputTextFlags_CallbackEdit) {
        auto pInputText = reinterpret_cast<InputText*>(data->UserData);

        std::string value = data->Buf;
        Widget::onInputTextChange_(pInputText->id, value);
    }

    return 0;
};

void Checkbox::Render(ReactImgui* view) {
    ImGui::PushID(id);
    if (ImGui::Checkbox(label.c_str(), &checked)) {
        printf("before\n");
        if (view->onBooleanValueChange != nullptr) {
            view->onBooleanValueChange(id, checked);
        } else {
            
        }
        printf("after\n");
    }
    ImGui::PopID();
};

void Button::Render(ReactImgui* view) {
    ImGui::PushID(id);
    if (ImGui::Button(label.c_str())) {
        view->onClick(id);
    }
    ImGui::PopID();
};

void Slider::Render(ReactImgui* view) {
    ImGui::PushID(id);
    if (type == "angle") {
        if (ImGui::SliderAngle(label.c_str(), &value, min, max, "%.0f")) {
            view->onNumericValueChange(id, value);
        }
    } else {
        if (ImGui::SliderFloat(label.c_str(), &value, min, max, "%.0f")) {
            view->onNumericValueChange(id, value);
        }
    }
    ImGui::PopID();
};

void MultiSlider::Render(ReactImgui* view) {
    ImGui::PushID(id);

    if (this->numValues == 2) {
        if (ImGui::SliderFloat2(this->label.c_str(), this->values.get(), this->min, this->max, view->floatFormatChars[this->decimalDigits].get())) {
            view->onMultiValueChange(this->id, this->values.get(), this->numValues);
        }
    } else if (this->numValues == 3) {
        if (ImGui::SliderFloat3(this->label.c_str(), values.get(), this->min, this->max, view->floatFormatChars[this->decimalDigits].get())) {
            view->onMultiValueChange(this->id, this->values.get(), this->numValues);
        }
    } else if (this->numValues == 4) {
        if (ImGui::SliderFloat4(this->label.c_str(), values.get(), this->min, this->max, view->floatFormatChars[this->decimalDigits].get())) {
            view->onMultiValueChange(this->id, this->values.get(), this->numValues);
        }
    }

    ImGui::PopID();
};

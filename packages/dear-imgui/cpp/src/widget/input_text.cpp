#include "widget/input_text.h"

#include <imgui.h>
#include <string>
#include <nlohmann/json.hpp>

#include "reactimgui.h"

void InputText::Render(ReactImgui* view) {
    ImGui::PushID(m_id);
    ImGui::InputText(m_label.c_str(), m_bufferPointer.get(), 100, inputTextFlags, InputTextCb, (void*)this);
    ImGui::PopID();
};

void InputText::Patch(const json& widgetPatchDef, ReactImgui* view) {
    if (widgetPatchDef.is_object()) {
        StyledWidget::Patch(widgetPatchDef, view);

        if (widgetPatchDef.contains("label") && widgetPatchDef["label"].is_string()) {
            m_label = widgetPatchDef["label"].template get<std::string>();
        }
    }
};

int InputText::InputTextCb(ImGuiInputTextCallbackData* data) {
    if (data->EventFlag == ImGuiInputTextFlags_CallbackEdit) {
        auto pInputText = reinterpret_cast<InputText*>(data->UserData);

        std::string value = data->Buf;
        onInputTextChange_(pInputText->m_id, value);
    }

    return 0;
};
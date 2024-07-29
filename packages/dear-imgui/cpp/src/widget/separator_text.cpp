#include "widget/separator_text.h"

#include <imgui.h>

#include "reactimgui.h"

void SeparatorText::Patch(const json& widgetPatchDef, ReactImgui* view) {
    if (widgetPatchDef.is_object()) {
        StyledWidget::Patch(widgetPatchDef, view);

        if (widgetPatchDef.contains("label") && widgetPatchDef["label"].is_string()) {
            m_label = widgetPatchDef["label"].template get<std::string>();
        }
    }
};

void SeparatorText::Render(ReactImgui* view) {
    ImGui::SeparatorText(m_label.c_str());
};
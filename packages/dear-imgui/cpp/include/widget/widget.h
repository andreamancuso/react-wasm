#include <string>
#include <nlohmann/json.hpp>

#include <reactimgui.h>
#include <element/element.h>

using json = nlohmann::json;

#pragma once

class ReactImgui;

class Widget : public Element {
    public:
        std::string m_type;

        // todo: does this belong here?
        inline static OnTextChangedCallback onInputTextChange_;

        explicit Widget(ReactImgui* view, int id);

        const char* GetElementType() override;

        void HandleChildren(ReactImgui* view) override;

        void SetChildrenDisplay(ReactImgui* view, YGDisplay display) const;

        void SetChildrenAsDisplayFlex(ReactImgui* view) const;

        void PreRender(ReactImgui* view) override;

        void Render(ReactImgui* view) override;

        void PostRender(ReactImgui* view) override;

        void Patch(const json& elementPatchDef, ReactImgui* view) override;
};


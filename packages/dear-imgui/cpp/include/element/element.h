#include <imgui_internal.h>
#include <sstream>
#include <nlohmann/json.hpp>

#include "layout_node.h"

using json = nlohmann::json;

#pragma once

class ReactImgui;

// For simplicity, only 1 'non-base' state at a time is allowed
enum ElementState {
    ElementState_Base = 0,
    ElementState_Disabled = 1,
    ElementState_Hover = 2,
    ElementState_Active = 3,
};

struct BorderStyle {
    ImVec4 color;
    float thickness = 0;
};

BorderStyle extractBorderStyle(const json& borderStyleDef);

struct BaseDrawStyle {
    std::optional<ImVec4> backgroundColor;
    std::optional<BorderStyle> borderTop;
    std::optional<BorderStyle> borderRight;
    std::optional<BorderStyle> borderBottom;
    std::optional<BorderStyle> borderLeft;
    std::optional<BorderStyle> borderAll;
    std::optional<float> rounding;
    ImDrawFlags drawFlags = ImDrawFlags_RoundCornersNone;
};

class Element {
    public:
        int m_id;
        std::string m_type;
        ReactImgui* m_view;
        bool m_handlesChildrenWithinRenderMethod;
        bool m_isRoot;
        bool m_cull;
        bool m_hovered;
        std::unique_ptr<LayoutNode> m_layoutNode;
        std::optional<BaseDrawStyle> m_baseDrawStyle;

        Element(ReactImgui* view, int id, bool isRoot, bool cull);

        static std::unique_ptr<Element> makeElement(const json& val, ReactImgui* view);

        const char* GetType() const;

        virtual void Init(const json& elementDef);

        virtual const char* GetElementType();

        virtual void HandleChildren(ReactImgui* view, const std::optional<ImRect>& parentViewport);

        bool ShouldRenderContent(const std::optional<ImRect>& viewport) const;

        virtual bool ShouldRender(ReactImgui* view) const;

        virtual void PreRender(ReactImgui* view);

        virtual void Render(ReactImgui* view, const std::optional<ImRect>& viewport);

        virtual void PostRender(ReactImgui* view);

        virtual ElementState GetState() const;

        void DrawBaseEffects() const;

        void ResetStyle();

        void SetStyle(const json& styleDef);

        ImRect GetScrollingAwareViewport();

        virtual void Patch(const json& elementPatchDef, ReactImgui* view);

        virtual bool HasInternalOps();

        virtual void HandleInternalOp(const json& opDef);

        virtual float GetLayoutLeftFromParentNode(YGNodeRef node, float left);

        virtual float GetLayoutTopFromParentNode(YGNodeRef node, float top);
};


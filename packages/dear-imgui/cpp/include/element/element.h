#include <imgui_internal.h>
#include <sstream>
#include <nlohmann/json.hpp>

#include "layout_node.h"

using json = nlohmann::json;

#pragma once

class ReactImgui;

struct BaseDrawStyle {
    std::optional<ImVec4> backgroundColor;
    std::optional<ImVec4> borderColor;
    std::optional<float> rounding;
    std::optional<float> borderThickness;
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


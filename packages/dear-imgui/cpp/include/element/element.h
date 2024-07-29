#include <cstring>
#include <sstream>
#include <nlohmann/json.hpp>

#include "layout_node.h"

using json = nlohmann::json;

#pragma once

class ReactImgui;

class Element {
    public:
        int m_id;
        ReactImgui* m_view;
        bool m_handlesChildrenWithinRenderMethod;
        bool m_isRoot;
        std::unique_ptr<LayoutNode> m_layoutNode;

        Element(ReactImgui* view, int id, bool isRoot);

        static std::unique_ptr<Element> makeElement(const json& val, ReactImgui* view);

        virtual void Init();

        virtual const char* GetElementType();

        virtual void HandleChildren(ReactImgui* view);

        virtual bool ShouldRender(ReactImgui* view) const;

        virtual void PreRender(ReactImgui* view);

        virtual void Render(ReactImgui* view);

        virtual void PostRender(ReactImgui* view);

        virtual void Patch(const json& elementPatchDef, ReactImgui* view);

        virtual float GetLayoutLeftFromParentNode(YGNodeRef node, float left);

        virtual float GetLayoutTopFromParentNode(YGNodeRef node, float top);
};


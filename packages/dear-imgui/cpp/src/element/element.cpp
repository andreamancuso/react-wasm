#include <cstring>
#include <nlohmann/json.hpp>

#include "shared.h"
#include "element/element.h"
#include "reactimgui.h"

using json = nlohmann::json;

Element::Element(ReactImgui* view, int id, bool isRoot) {
    m_id = id;
    m_view = view;
    m_handlesChildrenWithinRenderMethod = true;
    m_isRoot = isRoot;
    m_layoutNode = std::make_unique<LayoutNode>();
}

void Element::Init() {};

std::unique_ptr<Element> Element::makeElement(const json& nodeDef, ReactImgui* view) {
    auto id = nodeDef["id"].template get<int>();
    bool isRoot = (nodeDef.contains("root") && nodeDef["root"].is_boolean()) ? nodeDef["root"].template get<bool>() : false;
    auto element = std::make_unique<Element>(view, id, isRoot);

    if (nodeDef.is_object() && nodeDef.contains("style") && nodeDef["style"].is_object()) {
        element->m_layoutNode->ApplyStyle(nodeDef["style"]);
    }

    return element;
};

const char* Element::GetElementType() {
    return "node";
};

void Element::HandleChildren(ReactImgui* view) {
    view->RenderChildren(m_id);
};

float Element::GetLayoutLeftFromParentNode(YGNodeRef node, float left) {
    auto owner = YGNodeGetOwner(node);

    float currentNodeTop = YGNodeLayoutGetLeft(node);

    if (!YGFloatIsUndefined(currentNodeTop)) {
        left += currentNodeTop;
    }

    if (owner == nullptr) {
        return left;
    } else {
        return GetLayoutTopFromParentNode(owner, left);
    }
}

float Element::GetLayoutTopFromParentNode(YGNodeRef node, float top) {
    auto owner = YGNodeGetOwner(node);

    float currentNodeTop = YGNodeLayoutGetTop(node);

    if (!YGFloatIsUndefined(currentNodeTop)) {
        top += currentNodeTop;
    }

    if (owner == nullptr) {
        return top;
    } else {
        return GetLayoutTopFromParentNode(owner, top);
    }
}

void Element::Render(ReactImgui* view) {
    ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();

    YGNodeRef owner = YGNodeGetOwner(m_layoutNode->m_node);

    if (owner == nullptr) { // root
        YGNodeCalculateLayout(m_layoutNode->m_node, contentRegionAvail.x, contentRegionAvail.y, YGDirectionLTR);
    }

    const float left = YGNodeLayoutGetLeft(m_layoutNode->m_node);
    const float top = YGNodeLayoutGetTop(m_layoutNode->m_node);
    const float width = YGNodeLayoutGetWidth(m_layoutNode->m_node);
    const float height = YGNodeLayoutGetHeight(m_layoutNode->m_node);

    ImGui::SetCursorPos(ImVec2(left, top));

    ImGui::PushID(m_id);

    ImGui::BeginChild("##", ImVec2(width, height), ImGuiChildFlags_None);

    HandleChildren(view);

    ImGui::EndChild();
    ImGui::PopID();
};

bool Element::ShouldRender(ReactImgui* view) const {
    return LayoutNode::ShouldRender(m_layoutNode->m_node);
};

void Element::PreRender(ReactImgui* view) {};

void Element::PostRender(ReactImgui* view) {};

void Element::Patch(const json& nodeDef, ReactImgui* view) {
    if (nodeDef.is_object() && nodeDef.contains("style") && nodeDef["style"].is_object()) {
        m_layoutNode->ApplyStyle(nodeDef["style"]);
    }
};


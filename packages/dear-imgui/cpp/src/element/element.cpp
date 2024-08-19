#include <cstring>
#include <nlohmann/json.hpp>

#include "shared.h"
#include "element/element.h"

#include <imgui_internal.h>

#include "reactimgui.h"

using json = nlohmann::json;

Element::Element(ReactImgui* view, const int id, const bool isRoot, const bool cull) {
    m_type = "node";
    m_id = id;
    m_view = view;
    m_handlesChildrenWithinRenderMethod = true;
    m_isRoot = isRoot;
    m_cull = cull;
    m_layoutNode = std::make_unique<LayoutNode>();
}

void Element::Init(const json& elementDef) {
    YGNodeSetContext(m_layoutNode->m_node, this);

    if (elementDef.contains("style") && elementDef["style"].is_object()) {
        SetStyle(elementDef["style"]);
    }
};

const char* Element::GetType() const {
    return m_type.c_str();
};

std::unique_ptr<Element> Element::makeElement(const json& nodeDef, ReactImgui* view) {
    auto id = nodeDef["id"].template get<int>();
    bool isRoot = (nodeDef.contains("root") && nodeDef["root"].is_boolean()) ? nodeDef["root"].template get<bool>() : false;
    bool cull = (nodeDef.contains("cull") && nodeDef["cull"].is_boolean()) ? nodeDef["cull"].template get<bool>() : false;
    auto element = std::make_unique<Element>(view, id, isRoot, cull);

    return element;
};

void Element::ResetStyle() {
    m_layoutNode->ResetStyle();
    m_baseDrawStyle.reset();
};

void Element::SetStyle(const json& styleDef) {
    if (styleDef.is_object()) {
        m_layoutNode->ApplyStyle(styleDef);

        if (styleDef.contains("backgroundColor") || styleDef.contains("borderColor")) {
            BaseDrawStyle baseDrawStyle;

            if (styleDef.contains("backgroundColor")) {
                if (auto maybeColor = extractColor(styleDef["backgroundColor"]); maybeColor.has_value()) {
                    baseDrawStyle.backgroundColor = maybeColor.value();
                }
            }

            if (styleDef.contains("borderColor")) {
                if (auto maybeColor = extractColor(styleDef["borderColor"]); maybeColor.has_value()) {
                    baseDrawStyle.borderColor = maybeColor.value();
                }
            }

            if (baseDrawStyle.backgroundColor.has_value() || baseDrawStyle.borderColor.has_value()) {

                if (styleDef.contains("rounding")) {
                    baseDrawStyle.rounding = styleDef["rounding"].template get<float>();
                }

                if (styleDef.contains("borderThickness")) {
                    baseDrawStyle.borderThickness = styleDef["borderThickness"].template get<float>();
                }

                if (styleDef.contains("roundCorners") && styleDef["roundCorners"].is_array()) {
                    const auto roundCorners = styleDef["roundCorners"].template get<std::vector<std::string>>();

                    baseDrawStyle.drawFlags = std::accumulate(
                        roundCorners.begin(),
                        roundCorners.end(),
                        static_cast<ImDrawFlags>(ImDrawFlags_None),
                        cornersToDrawFlags
                    );
                }

                m_baseDrawStyle.emplace(baseDrawStyle);
            }
        }
    }
}

const char* Element::GetElementType() {
    return "node";
};

ImRect Element::GetScrollingAwareViewport() {
    auto innerClipRect = ImGui::GetCurrentWindow()->InnerClipRect;
    auto clippedSize = ImVec2(innerClipRect.Max.x - innerClipRect.Min.x, innerClipRect.Max.y - innerClipRect.Min.y);
    auto viewport = ImRect(ImGui::GetCurrentWindow()->Scroll.x, ImGui::GetCurrentWindow()->Scroll.y, ImGui::GetCurrentWindow()->Scroll.x + clippedSize.x, ImGui::GetCurrentWindow()->Scroll.y + clippedSize.y);

    // printf("%d %d %d %d\n", (int)viewport.Min.x, (int)viewport.Min.y, (int)viewport.Max.x, (int)viewport.Max.y);

    return viewport;
};

void Element::HandleChildren(ReactImgui* view, const std::optional<ImRect>& parentViewport) {
    if (parentViewport.has_value()) {
        // printf("%d has parent viewport\n", m_id);

        view->RenderChildren(m_id, parentViewport);
    } else if (m_cull) {
        auto viewport = GetScrollingAwareViewport();

        view->RenderChildren(m_id, viewport);
    } else {
        view->RenderChildren(m_id);
    }
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

void Element::Render(ReactImgui* view, const std::optional<ImRect>& viewport) {
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

    const auto size = ImVec2(width, height);

    ImGui::BeginChild("##", size, ImGuiChildFlags_None);

    if (m_baseDrawStyle.has_value()) {
        DrawBaseEffects();
    }

    HandleChildren(view, viewport);

    ImGui::EndChild();

    ImGui::PopID();
};

void Element::DrawBaseEffects() const {
    const float width = YGNodeLayoutGetWidth(m_layoutNode->m_node);
    const float height = YGNodeLayoutGetHeight(m_layoutNode->m_node);

    const auto size = ImVec2(width, height);

    if (size.x != 0 && size.y != 0) {
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        const ImVec2 p0 = ImGui::GetCursorScreenPos();
        const ImVec2 p1 = ImVec2(p0.x + width, p0.y + height);

        if (m_baseDrawStyle.value().backgroundColor.has_value()) {
            const ImU32 col = ImColor(m_baseDrawStyle.value().backgroundColor.value());

            drawList->AddRectFilled(
                p0,
                p1,
                col,
                m_baseDrawStyle.value().rounding.value_or(0),
                m_baseDrawStyle.value().drawFlags
            );
        }

        if (m_baseDrawStyle.value().borderColor.has_value()) {
            const ImU32 col = ImColor(m_baseDrawStyle.value().borderColor.value());

            drawList->AddRect(
                p0,
                p1,
                col,
                m_baseDrawStyle.value().rounding.value_or(0),
                ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersBottomRight,
                m_baseDrawStyle.value().borderThickness.value_or(1.0f)
            );
        }
    }
};

bool Element::ShouldRenderContent(const std::optional<ImRect>& viewport) const {
    if (m_type != "node") {
        // TODO: are we sure about this? Limits culling of content to nodes - not their contents. Hence, quite coarse at the moment.
        return true;
    }

    const float left = YGNodeLayoutGetLeft(m_layoutNode->m_node);
    const float top = YGNodeLayoutGetTop(m_layoutNode->m_node);
    const float width = YGNodeLayoutGetWidth(m_layoutNode->m_node);
    const float height = YGNodeLayoutGetHeight(m_layoutNode->m_node);

    if (viewport.has_value()) {
        // printf("%d %d %d %d\n", (int)viewport.value().Min.x, (int)viewport.value().Max.x, (int)viewport.value().Min.y, (int)viewport.value().Max.y);
        // if (left < viewport.value().Min.x || left > viewport.value().Max.x || top < viewport.value().Min.y || top > viewport.value().Max.y) {
        if ((left + width) < viewport.value().Min.x || (top + height) < viewport.value().Min.y) {
            // printf("Not rendering content (before) for %d\n", m_id);
            // Completely outside (before) the viewport
            return false;
        }

        if (left > viewport.value().Max.x || top > viewport.value().Max.y) {
            // printf("Not rendering content (after) for %d\n", m_id);
            // Completely outside (after) the viewport
            return false;
        }
    }

    return true;
}

bool Element::ShouldRender(ReactImgui* view) const {
    return LayoutNode::ShouldRender(m_layoutNode->m_node);
};

void Element::PreRender(ReactImgui* view) {};

void Element::PostRender(ReactImgui* view) {};

void Element::Patch(const json& nodeDef, ReactImgui* view) {
    if (nodeDef.contains("style") && nodeDef["style"].is_object()) {
        ResetStyle();
        SetStyle(nodeDef["style"]);
    }
};

bool Element::HasInternalOps() {
    return false;
};

void Element::HandleInternalOp(const json& opDef) {};
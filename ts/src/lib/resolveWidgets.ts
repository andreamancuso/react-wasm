import { ImguiWidget, ImguiWidgetsFlat, JSXWidgetNodesFlat } from "../components/ReactImgui/types";

// const unknownWidget: ImguiWidget<"Unknown"> = { type: "Unknown" };
const sameLineWidget: ImguiWidget<"SameLine", {}> = { type: "SameLine" };
const indentWidget: ImguiWidget<"Indent", {}> = { type: "Indent" };
const unindentWidget: ImguiWidget<"Unindent", {}> = { type: "Unindent" };

export const resolveWidget = (node: JSXWidgetNodesFlat, widgets: ImguiWidgetsFlat[] = []) => {
    const { props } = node;
    const widget: ImguiWidgetsFlat = props;

    switch (widget.type) {
        case "SameLine": {
            node.children?.forEach((subNode, index, arr) => {
                resolveWidget(subNode, widgets);

                if (index !== arr.length - 1) {
                    widgets.push(sameLineWidget);
                }
            });

            break;
        }

        case "Indent": {
            widgets.push(indentWidget);

            node.children?.forEach((subChild) => {
                resolveWidget(subChild, widgets);
            });

            widgets.push(unindentWidget);

            break;
        }

        case "TabBar":
        case "TabItem":
        case "Fragment":
        case "CollapsingHeader":
        case "TreeNode":
        case "ItemTooltip":
        case "TextWrap": {
            if (node.children) {
                widget.children = resolveWidgets(node.children);
            }

            widgets.push(widget);
            break;
        }

        default:
            widgets.push(widget);
            break;
    }
};

export const resolveWidgets = (
    children: JSXWidgetNodesFlat[],
    widgets: ImguiWidgetsFlat[] = [],
) => {
    children
        .filter((element) => element.type === "widget")
        .forEach((element) => {
            resolveWidget(element, widgets);
        });

    return widgets;
};

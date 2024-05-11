import { useWidgetEventManagement } from "src/lib/hooks/useWidgetEventManagement";
import { WidgetFunctionComponent, PropsWithChildren } from "./types";

export type TreeNodeProps = PropsWithChildren & {
    label?: string;
};

export const TreeNode: WidgetFunctionComponent<TreeNodeProps> = ({ children, label }) => {
    const [widgetId] = useWidgetEventManagement();

    return (
        <widget type="TreeNode" id={widgetId.current} label={label}>
            {children}
        </widget>
    );
};

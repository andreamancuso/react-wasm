import { useRef } from "react";
import { useWidgetRegistrationService } from "../../hooks";
import { WidgetFunctionComponent, PropsWithChildren, WidgetPropsMap } from "./types";

export const TreeNode: WidgetFunctionComponent<PropsWithChildren & WidgetPropsMap["TreeNode"]> = ({
    children,
    itemId,
    label,
    style,
    onClick,
    open,
    defaultOpen,
    defaultSelected,
    selectable,
    selected,
    leaf,
}) => {
    const widgetRegistratonService = useWidgetRegistrationService();
    const idRef = useRef(widgetRegistratonService.generateId());

    return (
        <widget
            type="TreeNode"
            itemId={itemId}
            id={idRef.current}
            label={label}
            onClick={onClick}
            open={open}
            selectable={selectable}
            selected={selected}
            defaultOpen={defaultOpen}
            defaultSelected={defaultSelected}
            leaf={leaf}
            style={style}
        >
            {leaf ? null : children}
        </widget>
    );
};

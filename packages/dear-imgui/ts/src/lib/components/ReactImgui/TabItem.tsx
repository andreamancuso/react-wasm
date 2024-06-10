import { useRef } from "react";
import { useWidgetRegistrationService } from "../../hooks";
import { PropsWithChildren, WidgetFunctionComponent, WidgetPropsMap } from "./types";

export type TabItemProps = PropsWithChildren & {
    label: string;
    onOpenChange?: (value: boolean) => void;
};

export const TabItem: WidgetFunctionComponent<PropsWithChildren & WidgetPropsMap["TabItem"]> = ({
    children,
    onOpenChange,
    label,
    style,
}) => {
    const widgetRegistratonService = useWidgetRegistrationService();
    const idRef = useRef(widgetRegistratonService.generateId());

    return (
        <widget type="TabItem" id={idRef.current} label={label} style={style}>
            {children}
        </widget>
    );
};

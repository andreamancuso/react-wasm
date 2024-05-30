import { useRef } from "react";
import { useWidgetRegistrationService } from "../../hooks";
import { PropsWithChildren, WidgetFunctionComponent, WidgetPropsMap } from "./types";

export const CollapsingHeader: WidgetFunctionComponent<PropsWithChildren & WidgetPropsMap["CollapsingHeader"]> = ({
    children,
    label,
}) => {
    const widgetRegistratonService = useWidgetRegistrationService();
    const idRef = useRef(widgetRegistratonService.generateId());

    return (
        <widget type="CollapsingHeader" id={idRef.current} label={label}>
            {children}
        </widget>
    );
};

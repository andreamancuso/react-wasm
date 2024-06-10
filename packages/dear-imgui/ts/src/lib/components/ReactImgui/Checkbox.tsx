import { useRef } from "react";
import { useWidgetRegistrationService } from "../../hooks";
import { WidgetFunctionComponent, WidgetPropsMap } from "./types";

export const Checkbox: WidgetFunctionComponent<WidgetPropsMap["Checkbox"]> = ({
    label,
    onChange,
    defaultChecked,
    style,
}) => {
    const widgetRegistratonService = useWidgetRegistrationService();
    const idRef = useRef(widgetRegistratonService.generateId());

    return (
        <widget
            type="Checkbox"
            label={label}
            id={idRef.current}
            defaultChecked={defaultChecked}
            onChange={onChange}
            style={style}
        />
    );
};

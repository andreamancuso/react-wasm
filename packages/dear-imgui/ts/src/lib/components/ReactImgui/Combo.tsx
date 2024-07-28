import { useMemo, useRef } from "react";
import { useWidgetRegistrationService } from "../../hooks";
import { WidgetFunctionComponent, WidgetPropsMap } from "./types";

export const Combo: WidgetFunctionComponent<WidgetPropsMap["Combo"]> = ({
    placeholder,
    options,
    onChange,
    initialSelectedIndex,
    style,
}) => {
    const widgetRegistratonService = useWidgetRegistrationService();
    const idRef = useRef(widgetRegistratonService.generateId());

    return (
        <widget
            type="Combo"
            placeholder={placeholder}
            id={idRef.current}
            initialSelectedIndex={initialSelectedIndex}
            options={options}
            onChange={onChange}
            style={style}
        />
    );
};

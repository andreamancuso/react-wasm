import { useRef } from "react";
import { useWidgetRegistrationService } from "../../hooks";
import { WidgetFunctionComponent, WidgetPropsMap } from "./types";

export const Slider: WidgetFunctionComponent<WidgetPropsMap["Slider"]> = ({
    label,
    min,
    max,
    onChange,
    defaultValue,
    sliderType = "default",
    style,
}) => {
    const widgetRegistratonService = useWidgetRegistrationService();
    const idRef = useRef(widgetRegistratonService.generateId());

    return (
        <slider
            label={label}
            id={idRef.current}
            defaultValue={defaultValue}
            min={min}
            max={max}
            sliderType={sliderType}
            onChange={onChange}
            style={style}
        />
    );
};

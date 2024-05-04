import { useEffect } from "react";
import { useWidgetEventManagement } from "../../hooks/useWidgetEventManagement";
import { WidgetFunctionComponent } from "./types";

type SliderProps = {
    label: string;
    defaultValue?: number;
    min?: number;
    max?: number;
    onChange?: (value: number) => void;
    valueType?: "default" | "angle";
};

export const Slider: WidgetFunctionComponent<SliderProps> = ({
    label,
    min,
    max,
    onChange,
    defaultValue,
    valueType = "default",
}) => {
    const [widgetId, widgetRegistrationService] = useWidgetEventManagement("numeric");

    useEffect(() => {
        if (onChange) {
            widgetRegistrationService.onNumericValueChange(widgetId.current, onChange);
        }
    }, [onChange]);

    return (
        <widget
            type="Slider"
            label={label}
            id={widgetId.current}
            defaultValue={defaultValue}
            min={min}
            max={max}
            valueType={valueType}
        />
    );
};

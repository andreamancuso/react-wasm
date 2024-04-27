import { useEffect } from "react";
import { useWidgetEventManagement } from "../../hooks/useWidgetEventManagement";
import { Primitive, WidgetFunctionComponent } from "./types";

type SliderProps = {
    label: string;
    defaultValues?: number[];
    min?: number;
    max?: number;
    numValues: 2 | 3 | 4;
    onChange?: <T extends Primitive>(...args: T[]) => void;
};

export const MultiSlider: WidgetFunctionComponent<SliderProps> = ({
    label,
    min,
    max,
    onChange,
    defaultValues,
    numValues,
}) => {
    const [widgetId, widgetRegistrationService] = useWidgetEventManagement("numeric");

    useEffect(() => {
        if (onChange) {
            widgetRegistrationService.onMultiValueChange(widgetId.current, onChange);
        }
    }, [onChange]);

    return (
        <widget
            type="MultiSlider"
            label={label}
            id={widgetId.current}
            defaultValues={defaultValues}
            min={min}
            max={max}
            numValues={numValues}
        />
    );
};

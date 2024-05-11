import { useEffect } from "react";
import { useWidgetEventManagement } from "src/lib/hooks/useWidgetEventManagement";
import { Primitive, WidgetFunctionComponent } from "./types";

type MultiSliderProps = {
    label?: string;
    defaultValues?: number[];
    min?: number;
    max?: number;
    decimalDigits?: number;
    numValues: 2 | 3 | 4;
    onChange?: (values: Primitive[]) => void;
};

export const MultiSlider: WidgetFunctionComponent<MultiSliderProps> = ({
    label,
    min,
    max,
    onChange,
    defaultValues,
    numValues,
    decimalDigits,
}) => {
    const [widgetId, widgetRegistrationService] = useWidgetEventManagement("multi");

    useEffect(() => {
        if (onChange) {
            widgetRegistrationService.onMultiValueChange(widgetId.current, onChange);
        }
    }, [onChange]);

    if (Array.isArray(defaultValues) && defaultValues.length !== numValues) {
        // todo: Mismatch! What to do?
    }

    return (
        <widget
            type="MultiSlider"
            label={label}
            id={widgetId.current}
            defaultValues={defaultValues}
            min={min}
            max={max}
            decimalDigits={decimalDigits}
            numValues={numValues}
            onChange={onChange}
        />
    );
};

import { useRef } from "react";
import { useWidgetRegistrationService } from "../../hooks";
import { SliderTypes, WidgetFunctionComponent, WidgetPropsMap } from "./types";

type SliderProps = {
    label: string;
    defaultValue?: number;
    min?: number;
    max?: number;
    onChange?: (value: number) => void;
    sliderType?: SliderTypes;
};

export const Slider: WidgetFunctionComponent<WidgetPropsMap["Slider"]> = ({
    label,
    min,
    max,
    onChange,
    defaultValue,
    sliderType = "default",
}: SliderProps) => {
    const widgetRegistratonService = useWidgetRegistrationService();
    const idRef = useRef(widgetRegistratonService.generateId());

    return (
        <widget
            type="Slider"
            label={label}
            id={idRef.current}
            defaultValue={defaultValue}
            min={min}
            max={max}
            sliderType={sliderType}
            onChange={onChange}
        />
    );
};

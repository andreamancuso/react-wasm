import { useEffect } from "react";
import { useWidgetEventManagement } from "../../hooks/useWidgetEventManagement";
import { WidgetFunctionComponent } from "./types";

type ComboProps = {
    label: string;
    options: { value: number; label: string }[];
    defaultValue?: number;
    onChange?: (value: number) => void;
};

export const Combo: WidgetFunctionComponent<ComboProps> = ({
    label,
    options,
    onChange,
    defaultValue,
}) => {
    const [widgetId, widgetRegistrationService] = useWidgetEventManagement("combo");

    useEffect(() => {
        if (onChange) {
            widgetRegistrationService.onComboChange(widgetId.current, onChange);
        }
    }, [onChange]);

    const optionsList = options.map(({ label }) => label).join(",");

    return (
        <widget
            type="Combo"
            label={label}
            id={widgetId.current}
            defaultValue={defaultValue}
            // Is it possible to pass arrays here?
            optionsList={optionsList}
            onChange={onChange}
        >
            {/* {options.map(({ value, label }) => (
                <div key={value} data-value={value} data-label={label} />
            ))} */}
        </widget>
    );
};

import { useEffect } from "react";
import { useWidgetEventManagement } from "../../hooks/useWidgetEventManagement";
import { WidgetFunctionComponent } from "./types";

type CheckboxProps = {
    label: string;
    defaultChecked?: boolean;
    onChange?: (value: boolean) => void;
};

export const Checkbox: WidgetFunctionComponent<CheckboxProps> = ({
    label,
    onChange,
    defaultChecked,
}) => {
    const [widgetId, widgetRegistrationService] = useWidgetEventManagement("boolean");

    useEffect(() => {
        if (onChange) {
            widgetRegistrationService.onBooleanValueChange(widgetId.current, onChange);
        }
    }, [onChange]);

    return (
        <widget
            type="Checkbox"
            label={label}
            id={widgetId.current}
            defaultChecked={defaultChecked}
        />
    );
};

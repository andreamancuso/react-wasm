import { useEffect } from "react";
import { useWidgetEventManagement } from "../../hooks/useWidgetEventManagement";
import { WidgetFunctionComponent } from "./types";

export type InputTextProps = {
    defaultValue?: string;
    label?: string;
    onChange?: (value: string) => void;
};

export const InputText: WidgetFunctionComponent<InputTextProps> = ({
    onChange,
    defaultValue,
    label,
}) => {
    const [widgetId, widgetRegistrationService] = useWidgetEventManagement("text");

    useEffect(() => {
        if (onChange) {
            widgetRegistrationService.onTextInputChange(widgetId.current, onChange);
        }
    }, [onChange]);

    return (
        <widget
            type="InputText"
            id={widgetId.current}
            defaultValue={defaultValue}
            label={label}
            onChange={onChange}
        />
    );
};

import { useRef } from "react";
import { useWidgetRegistrationService } from "../../hooks";
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
    const widgetRegistratonService = useWidgetRegistrationService();
    const idRef = useRef(widgetRegistratonService.generateId());

    return (
        <widget
            type="InputText"
            id={idRef.current}
            defaultValue={defaultValue}
            label={label}
            onChange={onChange}
        />
    );
};

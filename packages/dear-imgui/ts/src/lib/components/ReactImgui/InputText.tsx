import { forwardRef, useEffect, useImperativeHandle, useRef } from "react";
import { useWidgetRegistrationService } from "../../hooks";
import { WidgetPropsMap } from "./types";

export type InputTextImperativeHandle = {
    setValue: (value: string) => void;
};

export const InputText = forwardRef<InputTextImperativeHandle, WidgetPropsMap["InputText"]>(
    ({ onChange, defaultValue, label, style }, ref) => {
        const widgetRegistratonService = useWidgetRegistrationService();
        const idRef = useRef(widgetRegistratonService.generateId());

        useImperativeHandle(
            ref,
            () => {
                return {
                    setValue(value: string) {
                        widgetRegistratonService.setInputTextValue(idRef.current, value);
                    },
                };
            },
            [],
        );

        return (
            <widget
                type="InputText"
                id={idRef.current}
                defaultValue={defaultValue}
                label={label}
                onChange={onChange}
                style={style}
            />
        );
    },
);

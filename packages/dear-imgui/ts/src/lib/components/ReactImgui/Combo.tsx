import { forwardRef, useImperativeHandle, useRef } from "react";
import { useWidgetRegistrationService } from "../../hooks";
import { WidgetPropsMap } from "./types";

export type ComboImperativeHandle = {
    setSelectedIndex: (index: number) => void;
};

export const Combo = forwardRef<ComboImperativeHandle, WidgetPropsMap["Combo"]>(
    ({ placeholder, options, onChange, initialSelectedIndex, style }, ref) => {
        const widgetRegistratonService = useWidgetRegistrationService();
        const idRef = useRef(widgetRegistratonService.generateId());

        useImperativeHandle(
            ref,
            () => {
                return {
                    setSelectedIndex(index: number) {
                        widgetRegistratonService.setComboSelectedIndex(idRef.current, index);
                    },
                };
            },
            [],
        );

        return (
            <widget
                type="Combo"
                placeholder={placeholder}
                id={idRef.current}
                initialSelectedIndex={initialSelectedIndex}
                options={options}
                onChange={onChange}
                style={style}
            />
        );
    },
);

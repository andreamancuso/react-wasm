import { useMemo, useRef } from "react";
import { useWidgetRegistrationService } from "../../hooks";
import { WidgetFunctionComponent, WidgetPropsMap } from "./types";

export const Combo: WidgetFunctionComponent<WidgetPropsMap["Combo"]> = ({
    label,
    options,
    onChange,
    defaultValue,
}) => {
    const widgetRegistratonService = useWidgetRegistrationService();
    const idRef = useRef(widgetRegistratonService.generateId());

    const optionsList = useMemo(() => options?.map(({ label }) => label).join(","), [options]);

    return (
        <widget
            type="Combo"
            label={label}
            id={idRef.current}
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

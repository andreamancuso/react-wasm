import { useRef } from "react";
import { useWidgetRegistrationService } from "../../hooks";
import { PropsWithChildren, WidgetFunctionComponent } from "./types";

export type CollapsingHeaderProps = PropsWithChildren & {
    label: string;
    onChange?: (value: boolean) => void;
};

export const CollapsingHeader: WidgetFunctionComponent<CollapsingHeaderProps> = ({
    children,
    label,
    onChange,
}) => {
    const widgetRegistratonService = useWidgetRegistrationService();
    const idRef = useRef(widgetRegistratonService.generateId());

    return (
        <widget type="CollapsingHeader" id={idRef.current} label={label} onChange={onChange}>
            {children}
        </widget>
    );
};

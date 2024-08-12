import { useRef } from "react";
import { useWidgetRegistrationService } from "../../hooks";
import { WidgetFunctionComponent, WidgetPropsMap } from "./types";

export const Button: WidgetFunctionComponent<WidgetPropsMap["Button"]> = ({
    label,
    style,
    onClick,
}) => {
    const widgetRegistratonService = useWidgetRegistrationService();
    const idRef = useRef(widgetRegistratonService.generateId());

    return <di-button label={label} id={idRef.current} onClick={onClick} style={style} />;
};

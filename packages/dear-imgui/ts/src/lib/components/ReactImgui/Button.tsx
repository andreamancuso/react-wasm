import { useRef } from "react";
import { useWidgetRegistrationService } from "../../hooks";
import { WidgetFunctionComponent, WidgetPropsMap } from "./types";

export const Button: WidgetFunctionComponent<WidgetPropsMap["Button"]> = ({ label, onClick }) => {
    const widgetRegistratonService = useWidgetRegistrationService();
    const idRef = useRef(widgetRegistratonService.generateId());

    return <widget type="Button" label={label} id={idRef.current} onClick={onClick} />;
};

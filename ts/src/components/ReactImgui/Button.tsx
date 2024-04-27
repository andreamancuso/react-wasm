import { useEffect } from "react";
import { useWidgetEventManagement } from "../../hooks/useWidgetEventManagement";
import { WidgetFunctionComponent } from "./types";

type ButtonProps = {
    label: string;
    defaultChecked?: boolean;
    onClick?: () => void;
};

export const Button: WidgetFunctionComponent<ButtonProps> = ({ label, onClick }) => {
    const [widgetId, widgetRegistrationService] = useWidgetEventManagement("boolean");

    useEffect(() => {
        if (onClick) {
            widgetRegistrationService.onBooleanValueChange(widgetId.current, onClick);
        }
    }, [onClick]);

    return <widget type="Button" label={label} id={widgetId.current} />;
};

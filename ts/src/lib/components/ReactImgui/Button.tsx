import { useEffect } from "react";
import { useWidgetEventManagement } from "src/lib/hooks/useWidgetEventManagement";
import { WidgetFunctionComponent } from "./types";

type ButtonProps = {
    label: string;
    defaultChecked?: boolean;
    onClick?: () => void;
};

export const Button: WidgetFunctionComponent<ButtonProps> = ({ label, onClick }) => {
    const [widgetId, widgetRegistrationService] = useWidgetEventManagement();

    return <widget type="Button" label={label} id={widgetId.current} onClick={onClick} />;
};

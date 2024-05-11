import { useEffect } from "react";
import { useWidgetEventManagement } from "src/lib/hooks/useWidgetEventManagement";
import { PropsWithChildren, WidgetFunctionComponent } from "./types";

export type TabItemProps = PropsWithChildren & {
    label: string;
    onOpenChange?: (value: boolean) => void;
};

export const TabItem: WidgetFunctionComponent<TabItemProps> = ({
    children,
    onOpenChange,
    label,
}) => {
    const [widgetId, widgetRegistrationService] = useWidgetEventManagement("boolean");

    useEffect(() => {
        if (onOpenChange) {
            widgetRegistrationService.onBooleanValueChange(widgetId.current, onOpenChange);
        }
    }, [onOpenChange]);

    return (
        <widget type="TabItem" id={widgetId.current} label={label}>
            {children}
        </widget>
    );
};

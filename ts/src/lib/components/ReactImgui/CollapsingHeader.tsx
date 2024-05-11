import { useWidgetEventManagement } from "src/lib/hooks/useWidgetEventManagement";
import { PropsWithChildren, WidgetFunctionComponent } from "./types";

export type CollapsingHeaderProps = PropsWithChildren & {
    label?: string;
};

export const CollapsingHeader: WidgetFunctionComponent<CollapsingHeaderProps> = ({
    children,
    label,
}) => {
    const [widgetId] = useWidgetEventManagement();

    return (
        <widget type="CollapsingHeader" id={widgetId.current} label={label}>
            {children}
        </widget>
    );
};

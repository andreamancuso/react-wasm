import { useMemo } from "react";
import { useWidgetEventManagement } from "src/lib/hooks/useWidgetEventManagement";
import { PropsWithChildren, WidgetFunctionComponent, WidgetReactElement } from "./types";
import { TabItem } from "./TabItem";

export const TabBar: WidgetFunctionComponent<PropsWithChildren> = ({ children }) => {
    const [widgetId, widgetRegistrationService] = useWidgetEventManagement();

    const tabs = useMemo(
        () =>
            Array.isArray(children)
                ? children.filter(
                      (child): child is WidgetReactElement<"TabItem"> => child.type === TabItem,
                  )
                : [],
        [children],
    );

    return (
        <widget type="TabBar" id={widgetId.current}>
            {tabs}
        </widget>
    );
};

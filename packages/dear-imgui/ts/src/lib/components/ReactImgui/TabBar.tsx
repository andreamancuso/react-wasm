import { useMemo, useRef } from "react";
import { useWidgetRegistrationService } from "../../hooks";
import { PropsWithChildren, WidgetFunctionComponent, WidgetReactElement } from "./types";
import { TabItem } from "./TabItem";

export const TabBar: WidgetFunctionComponent<PropsWithChildren> = ({ children }) => {
    const widgetRegistratonService = useWidgetRegistrationService();
    const idRef = useRef(widgetRegistratonService.generateId());

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
        <widget type="TabBar" id={idRef.current}>
            {tabs}
        </widget>
    );
};

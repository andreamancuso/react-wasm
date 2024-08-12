import { useMemo, useRef } from "react";
import { useWidgetRegistrationService } from "../../hooks";
import {
    PropsWithChildren,
    WidgetFunctionComponent,
    WidgetPropsMap,
    WidgetReactElement,
} from "./types";
import { TabItem } from "./TabItem";

export const TabBar: WidgetFunctionComponent<PropsWithChildren & WidgetPropsMap["TabBar"]> = ({
    children,
    style,
}) => {
    const widgetRegistratonService = useWidgetRegistrationService();
    const idRef = useRef(widgetRegistratonService.generateId());

    const tabs = useMemo(() => {
        if (children) {
            const localChildren = Array.isArray(children) ? children : [children];

            return localChildren.filter(
                (child): child is WidgetReactElement<"TabItem"> => child.type === TabItem,
            );
        }

        return [];
    }, [children]);

    return (
        <tab-bar id={idRef.current} style={style}>
            {tabs}
        </tab-bar>
    );
};

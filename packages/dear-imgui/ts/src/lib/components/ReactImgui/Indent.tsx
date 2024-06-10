import { WidgetPropsMap, WidgetFunctionComponent, PropsWithChildren } from "./types";

export const Indent: WidgetFunctionComponent<PropsWithChildren & WidgetPropsMap["Indent"]> = ({
    children,
    style,
}) => {
    return (
        <widget type="Indent" style={style}>
            {children}
        </widget>
    );
};

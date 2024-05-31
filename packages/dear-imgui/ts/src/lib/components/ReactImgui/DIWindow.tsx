import { PropsWithChildren, WidgetFunctionComponent, WidgetPropsMap } from "./types";

// todo: perhaps we can come up with a way to deal with 'real' fragments
export const DIWindow: WidgetFunctionComponent<PropsWithChildren & WidgetPropsMap["DIWindow"]> = ({
    title,
    children,
}) => {
    return (
        <widget type="DIWindow" title={title}>
            {children}
        </widget>
    );
};

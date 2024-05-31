import { PropsWithChildren, WidgetFunctionComponent, WidgetPropsMap } from "./types";

// todo: perhaps we can come up with a way to deal with 'real' fragments
export const DIWindow: WidgetFunctionComponent<PropsWithChildren & WidgetPropsMap["DIWindow"]> = ({
    title,
    width,
    height,
    children,
}) => {
    return (
        <widget type="DIWindow" width={width} height={height} title={title}>
            {children}
        </widget>
    );
};

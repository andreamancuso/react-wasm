import { PropsWithChildren, WidgetFunctionComponent, WidgetPropsMap } from "./types";

// todo: perhaps we can come up with a way to deal with 'real' fragments
export const Child: WidgetFunctionComponent<PropsWithChildren & WidgetPropsMap["Child"]> = ({
    children,
    style,
}) => {
    return <child style={style}>{children}</child>;
};

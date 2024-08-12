import { PropsWithChildren, WidgetFunctionComponent, WidgetPropsMap } from "./types";

export const TextWrap: WidgetFunctionComponent<PropsWithChildren & WidgetPropsMap["TextWrap"]> = ({
    children,
    width,
    style,
}) => {
    return (
        <text-wrap width={width} style={style}>
            {children}
        </text-wrap>
    );
};

import { PropsWithChildren, WidgetFunctionComponent, WidgetPropsMap } from "./types";

export const TextWrap: WidgetFunctionComponent<PropsWithChildren & WidgetPropsMap["TextWrap"]> = ({
    children,
    width,
    style,
}) => {
    return (
        <widget type="TextWrap" width={width} style={style}>
            {children}
        </widget>
    );
};

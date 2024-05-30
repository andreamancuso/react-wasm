import { PropsWithChildren, WidgetFunctionComponent, WidgetPropsMap } from "./types";

export const TextWrap: WidgetFunctionComponent<PropsWithChildren & WidgetPropsMap["TextWrap"]> = ({
    children,
    width,
}) => {
    return (
        <widget type="TextWrap" width={width}>
            {children}
        </widget>
    );
};

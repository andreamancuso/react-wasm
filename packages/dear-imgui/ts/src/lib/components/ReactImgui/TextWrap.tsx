import { PropsWithChildren, WidgetFunctionComponent } from "./types";

type TextWrapProps = PropsWithChildren & {
    width: number;
};

export const TextWrap: WidgetFunctionComponent<TextWrapProps> = ({ children, width }) => {
    return (
        <widget type="TextWrap" width={width}>
            {children}
        </widget>
    );
};

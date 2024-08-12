import { WidgetFunctionComponent, WidgetPropsMap } from "./types";

export const DisabledText: WidgetFunctionComponent<WidgetPropsMap["DisabledText"]> = ({
    text,
    style,
}) => {
    return <disabled-text text={text} style={style} />;
};

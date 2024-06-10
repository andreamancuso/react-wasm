import { WidgetFunctionComponent, WidgetPropsMap } from "./types";

export const DisabledText: WidgetFunctionComponent<WidgetPropsMap["DisabledText"]> = ({
    text,
    style,
}) => {
    return <widget type="DisabledText" text={text} style={style} />;
};

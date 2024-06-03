import { WidgetFunctionComponent, WidgetPropsMap } from "./types";

export const UnformattedText: WidgetFunctionComponent<WidgetPropsMap["UnformattedText"]> = ({
    text,
    style,
}) => {
    return <widget type="UnformattedText" text={text} style={style} />;
};

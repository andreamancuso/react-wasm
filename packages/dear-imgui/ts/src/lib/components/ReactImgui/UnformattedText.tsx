import { WidgetFunctionComponent, WidgetPropsMap } from "./types";

export const UnformattedText: WidgetFunctionComponent<WidgetPropsMap["UnformattedText"]> = ({
    text,
    font,
    color,
}) => {
    return <widget type="UnformattedText" text={text} font={font} color={color} />;
};

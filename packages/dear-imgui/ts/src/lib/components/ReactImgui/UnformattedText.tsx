import { WidgetFunctionComponent, WidgetPropsMap } from "./types";

export const UnformattedText: WidgetFunctionComponent<WidgetPropsMap["UnformattedText"]> = ({
    text,
    style,
}) => {
    return <unformatted-text text={text} style={style} />;
};

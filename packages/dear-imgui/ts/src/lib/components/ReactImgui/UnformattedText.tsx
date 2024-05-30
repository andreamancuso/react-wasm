import { WidgetFunctionComponent, WidgetPropsMap } from "./types";

export const UnformattedText: WidgetFunctionComponent<WidgetPropsMap["UnformattedText"]> = ({
    text,
}) => {
    return <widget type="UnformattedText" text={text} />;
};

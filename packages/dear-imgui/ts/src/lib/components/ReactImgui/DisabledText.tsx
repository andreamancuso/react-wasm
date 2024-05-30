import { WidgetFunctionComponent, WidgetPropsMap } from "./types";

export const DisabledText: WidgetFunctionComponent<WidgetPropsMap["DisabledText"]> = ({ text }) => {
    return <widget type="DisabledText" text={text} />;
};

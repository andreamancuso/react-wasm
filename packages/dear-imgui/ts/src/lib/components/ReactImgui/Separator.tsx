import { WidgetFunctionComponent, WidgetPropsMap } from "./types";

export const Separator: WidgetFunctionComponent<WidgetPropsMap["Separator"]> = ({ style }) => {
    return <separator style={style} />;
};

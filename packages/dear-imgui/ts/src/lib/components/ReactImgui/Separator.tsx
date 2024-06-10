import { WidgetFunctionComponent, WidgetPropsMap } from "./types";

export const Separator: WidgetFunctionComponent<WidgetPropsMap["Separator"]> = ({ style }) => {
    return <widget type="Separator" style={style} />;
};

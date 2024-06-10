import { WidgetFunctionComponent, WidgetPropsMap } from "./types";

export type SeparatorTextProps = {
    label: string;
};

export const SeparatorText: WidgetFunctionComponent<WidgetPropsMap["SeparatorText"]> = ({
    label,
    style,
}) => {
    return <widget type="SeparatorText" label={label} style={style} />;
};

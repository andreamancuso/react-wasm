import { WidgetFunctionComponent, WidgetPropsMap } from "./types";

export type SeparatorTextProps = {
    label: string;
};

export const SeparatorText: WidgetFunctionComponent<WidgetPropsMap["SeparatorText"]> = ({
    label,
    style,
}) => {
    return <separator-text label={label} style={style} />;
};

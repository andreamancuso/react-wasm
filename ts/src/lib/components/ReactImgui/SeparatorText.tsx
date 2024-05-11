import { WidgetFunctionComponent } from "./types";

export type SeparatorTextProps = {
    label: string;
};

export const SeparatorText: WidgetFunctionComponent<SeparatorTextProps> = ({ label }) => {
    return <widget type="SeparatorText" label={label} />;
};

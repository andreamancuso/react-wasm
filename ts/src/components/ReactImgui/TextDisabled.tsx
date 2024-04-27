import { WidgetFunctionComponent } from "./types";

export type TextDisabledProps = {
    text: string;
};

export const TextDisabled: WidgetFunctionComponent<TextDisabledProps> = ({ text }) => {
    return <widget type="TextDisabled" text={text} />;
};

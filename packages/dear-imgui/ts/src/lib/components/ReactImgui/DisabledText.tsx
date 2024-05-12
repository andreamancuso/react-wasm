import { WidgetFunctionComponent } from "./types";

export type DisabledTextProps = {
    text: string;
};

export const DisabledText: WidgetFunctionComponent<DisabledTextProps> = ({ text }) => {
    return <widget type="DisabledText" text={text} />;
};

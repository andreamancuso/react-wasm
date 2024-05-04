import { WidgetFunctionComponent } from "./types";

export type UnformattedTextProps = {
    text: string;
};

export const UnformattedText: WidgetFunctionComponent<UnformattedTextProps> = ({ text }) => {
    return <widget type="UnformattedText" text={text} />;
};

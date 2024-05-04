import { WidgetFunctionComponent } from "./types";

export type BulletTextProps = {
    text: string;
};

export const BulletText: WidgetFunctionComponent<BulletTextProps> = ({ text }) => {
    return <widget type="BulletText" text={text} />;
};

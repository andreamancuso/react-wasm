import { WidgetFunctionComponent, WidgetPropsMap } from "./types";

export const BulletText: WidgetFunctionComponent<WidgetPropsMap["BulletText"]> = ({ text }) => {
    return <widget type="BulletText" text={text} />;
};

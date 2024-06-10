import { WidgetFunctionComponent, WidgetPropsMap } from "./types";

export const BulletText: WidgetFunctionComponent<WidgetPropsMap["BulletText"]> = ({
    text,
    style,
}) => {
    return <widget type="BulletText" text={text} style={style} />;
};

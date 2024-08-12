import { WidgetFunctionComponent, WidgetPropsMap } from "./types";

export const BulletText: WidgetFunctionComponent<WidgetPropsMap["BulletText"]> = ({
    text,
    style,
}) => {
    return <bullet-text text={text} style={style} />;
};

import { PropsWithChildren, WidgetFunctionComponent, WidgetPropsMap } from "./types";

export const ItemTooltip: WidgetFunctionComponent<
    PropsWithChildren & WidgetPropsMap["ItemTooltip"]
> = ({ children, style }) => {
    return (
        <widget type="ItemTooltip" style={style}>
            {children}
        </widget>
    );
};

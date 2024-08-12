import { PropsWithChildren, WidgetFunctionComponent, WidgetPropsMap } from "./types";

export const ItemTooltip: WidgetFunctionComponent<
    PropsWithChildren & WidgetPropsMap["ItemTooltip"]
> = ({ children, style }) => {
    return <item-tooltip style={style}>{children}</item-tooltip>;
};

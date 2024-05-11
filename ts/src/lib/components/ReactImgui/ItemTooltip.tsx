import { PropsWithChildren, WidgetFunctionComponent } from "./types";

export const ItemTooltip: WidgetFunctionComponent<PropsWithChildren> = ({ children }) => {
    return <widget type="ItemTooltip">{children}</widget>;
};

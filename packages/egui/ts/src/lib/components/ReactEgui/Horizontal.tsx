import { WidgetFunctionComponent, PropsWithChildren } from "./types";

export const Horizontal: WidgetFunctionComponent<PropsWithChildren> = ({ children }) => {
    return <widget type="Horizontal">{children}</widget>;
};

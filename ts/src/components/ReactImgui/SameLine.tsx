import { WidgetFunctionComponent, PropsWithChildren } from "./types";

export const SameLine: WidgetFunctionComponent<PropsWithChildren> = ({ children }) => {
    return <widget type="SameLine">{children}</widget>;
};

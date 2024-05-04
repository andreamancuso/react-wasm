import { PropsWithChildren, WidgetFunctionComponent } from "./types";

export const Indent: WidgetFunctionComponent<PropsWithChildren> = ({ children }) => {
    return <widget type="Indent">{children}</widget>;
};

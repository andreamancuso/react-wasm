import { PropsWithChildren, WidgetFunctionComponent } from "./types";

// todo: perhaps we can come up with a way to deal with 'real' fragments
export const Fragment: WidgetFunctionComponent<PropsWithChildren> = ({ children }) => {
    return <widget type="Fragment">{children}</widget>;
};

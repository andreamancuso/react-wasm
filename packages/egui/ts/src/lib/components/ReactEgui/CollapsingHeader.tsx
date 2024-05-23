import { PropsWithChildren, WidgetFunctionComponent } from "./types";

export type CollapsingHeaderProps = PropsWithChildren & {
    label: string;
    onChange?: (value: boolean) => void;
};

export const CollapsingHeader: WidgetFunctionComponent<CollapsingHeaderProps> = ({
    children,
    label,
    onChange,
}) => {
    return (
        <widget type="CollapsingHeader" label={label} onChange={onChange}>
            {children}
        </widget>
    );
};

import { WidgetFunctionComponent } from "./types";

export type InputTextProps = {
    defaultValue?: string;
    label?: string;
    onChange?: (value: string) => void;
};

export const InputText: WidgetFunctionComponent<InputTextProps> = ({
    onChange,
    defaultValue,
    label,
}) => {
    return (
        <widget type="InputText" defaultValue={defaultValue} label={label} onChange={onChange} />
    );
};

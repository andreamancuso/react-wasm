import { WidgetFunctionComponent } from "./types";

type CheckboxProps = {
    label: string;
    tooltipText?: string;
    defaultChecked?: boolean;
    onChange?: (value: boolean) => void;
};

export const Checkbox: WidgetFunctionComponent<CheckboxProps> = ({
    label,
    tooltipText,
    onChange,
    defaultChecked,
}) => {
    return (
        <widget
            type="Checkbox"
            label={label}
            tooltipText={tooltipText}
            defaultChecked={defaultChecked}
            onChange={onChange}
        />
    );
};

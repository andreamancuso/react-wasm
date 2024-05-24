import { WidgetFunctionComponent } from "./types";

type RadioButtonProps = {
    label: string;
    tooltipText?: string;
    defaultChecked?: boolean;
    value?: string | number;
    onChange?: (value: boolean) => void;
};

export const RadioButton: WidgetFunctionComponent<RadioButtonProps> = ({
    label,
    tooltipText,
    onChange,
    defaultChecked,
    value,
}) => {
    return (
        <widget
            type="RadioButton"
            label={label}
            tooltipText={tooltipText}
            defaultChecked={defaultChecked}
            onChange={onChange}
            value={value}
        />
    );
};

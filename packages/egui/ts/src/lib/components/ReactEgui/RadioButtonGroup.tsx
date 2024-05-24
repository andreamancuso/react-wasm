import { WidgetFunctionComponent } from "./types";

type RadioButtonGroupProps = {
    options: { label: string; value: string; tooltipText?: string }[];
    defaultValue?: string;
    onChange?: (value: string) => void;
};

export const RadioButtonGroup: WidgetFunctionComponent<RadioButtonGroupProps> = ({
    onChange,
    defaultValue,
    options,
}) => {
    return (
        <widget
            type="RadioButtonGroup"
            onChange={onChange}
            defaultValue={defaultValue}
            options={options}
        />
    );
};

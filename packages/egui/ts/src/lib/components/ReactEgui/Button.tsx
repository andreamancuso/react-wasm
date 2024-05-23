import { WidgetFunctionComponent } from "./types";

type ButtonProps = {
    label: string;
    defaultChecked?: boolean;
    onClick?: () => void;
};

export const Button: WidgetFunctionComponent<ButtonProps> = ({ label, onClick }) => {
    return <widget type="Button" label={label} onClick={onClick} />;
};

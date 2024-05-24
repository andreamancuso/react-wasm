import { MainComponent } from "../ReactEgui";
import { attachSubComponents } from "../../attachSubComponents";
import { Button } from "./Button";
import { InputText } from "./InputText";
import { Horizontal } from "./Horizontal";
import { CollapsingHeader } from "./CollapsingHeader";
import { Checkbox } from "./Checkbox";
import { RadioButton } from "./RadioButton";
import { RadioButtonGroup } from "./RadioButtonGroup";

export const components = {
    InputText,
    Button,
    Horizontal,
    CollapsingHeader,
    Checkbox,
    RadioButton,
    RadioButtonGroup,
} as const;

export const ReactEgui = attachSubComponents("ReactEgui", MainComponent, components);

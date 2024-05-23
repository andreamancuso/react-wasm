import { MainComponent } from "../ReactEgui";
import { attachSubComponents } from "../../attachSubComponents";
import { Button } from "./Button";
import { InputText } from "./InputText";
import { Horizontal } from "./Horizontal";
import { CollapsingHeader } from "./CollapsingHeader";

export const components = {
    InputText,
    Button,
    Horizontal,
    CollapsingHeader,
} as const;

export const ReactEgui = attachSubComponents("ReactEgui", MainComponent, components);

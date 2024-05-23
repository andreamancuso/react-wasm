import { MainComponent } from "../ReactEgui";
import { attachSubComponents } from "../../attachSubComponents";
import { Button } from "./Button";
import { InputText } from "./InputText";
import { Horizontal } from "./Horizontal";

export const components = {
    InputText,
    Button,
    Horizontal,
} as const;

export const ReactEgui = attachSubComponents("ReactEgui", MainComponent, components);

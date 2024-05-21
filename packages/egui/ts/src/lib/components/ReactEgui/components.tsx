import { MainComponent } from "../ReactEgui";
import { attachSubComponents } from "../../attachSubComponents";
import { Button } from "./Button";
import { InputText } from "./InputText";

export const components = {
    InputText,
    Button,
} as const;

export const ReactEgui = attachSubComponents("ReactEgui", MainComponent, components);

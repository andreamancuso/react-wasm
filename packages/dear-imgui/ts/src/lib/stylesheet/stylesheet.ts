import { ImGuiCol, ImGuiStyleVar, ImVec2 } from "../wasm/wasm-app-types";
import { StyleColValue, StyleVarValue } from "./imgui-style";

export type StyleRules = {
    font?: {
        name: string;
        size: number;
    };
    colors?: { [k in ImGuiCol]?: StyleColValue };
    vars?: { [k in ImGuiStyleVar]?: StyleVarValue };
};

export type RWStyleSheet = {
    [k: string]: StyleRules;
};

const RWStyleSheet = {
    create: (styleSheet: RWStyleSheet): RWStyleSheet => styleSheet,
};

export default RWStyleSheet;

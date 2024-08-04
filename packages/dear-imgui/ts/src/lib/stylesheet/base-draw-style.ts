import { StyleColValue } from "./types";

export type RoundCorners = "all" | "topLeft" | "topRight" | "bottomLeft" | "bottomRight";

export type BaseDrawStyle = {
    backgroundColor?: StyleColValue;
    borderColor?: StyleColValue;
    rounding?: number;
    borderThickness?: number;
    roundCorners?: RoundCorners[];
};

import * as React from "react";
import { ReactImgui } from "./ReactImgui";

export const App = () => {
    return (
        <ReactImgui.Node root style={{ width: "100%", height: "100%" }}>
            <ReactImgui.InputText />
            <ReactImgui.Button label="Click me" />
        </ReactImgui.Node>
    );
};

import React, { useRef } from "react";
import { ReactImgui, ImGuiDemo } from "@react-wasm/dear-imgui";
// @ts-ignore
import getWasmModule from "@react-wasm/dear-imgui/dist/reactDearImgui.mjs";

import "./App.css";

function App() {
    const containerRef = useRef<HTMLDivElement>(null);

    return (
        <div id="app" ref={containerRef}>
            <ReactImgui getWasmModule={getWasmModule} containerRef={containerRef}>
                <ImGuiDemo />
            </ReactImgui>
        </div>
    );
}

export default App;

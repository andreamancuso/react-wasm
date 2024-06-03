import React, { useMemo, useRef } from "react";
import { ReactImgui, ImGuiDemo } from "@react-wasm/dear-imgui";
// @ts-ignore
import getWasmModule from "@react-wasm/dear-imgui/dist/reactDearImgui.mjs";
// @ts-ignore
import wasmDataPackage from "@react-wasm/dear-imgui/dist/reactDearImgui.data";

import "./App.css";

function App() {
    const containerRef = useRef<HTMLDivElement>(null);

    const fontDefs = useMemo(
        () => [
            { name: "roboto-regular", sizes: [12, 14, 16, 18, 24] },
            { name: "roboto-bold", sizes: [12, 14, 16, 18, 24] },
            { name: "roboto-light", sizes: [12, 14, 16, 18, 24] },
            { name: "roboto-mono-regular", sizes: [14] },
        ],
        [],
    );

    const defaultFont = useMemo(() => ({ name: "roboto-regular", size: 16 }), []);

    return (
        <div id="app" ref={containerRef}>
            <ReactImgui
                wasmDataPackage={wasmDataPackage}
                getWasmModule={getWasmModule}
                containerRef={containerRef}
                fontDefs={fontDefs}
                defaultFont={defaultFont}
            >
                <ImGuiDemo />
            </ReactImgui>
        </div>
    );
}

export default App;

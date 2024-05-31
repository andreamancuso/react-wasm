import { useRef } from "react";
import { ReactImgui } from "./lib/components/ReactImgui/components";
// @ts-ignore
import getWasmModule from "./lib/wasm/reactDearImgui.mjs";
// @ts-ignore
import wasmDataPackage from "./lib/wasm/reactDearImgui.data";
import { ImGuiDemo } from "./lib";

import "./App.css";

// console.log(process.env.NODE_ENV);

function App() {
    const containerRef = useRef<HTMLDivElement>(null);

    return (
        <div id="app" ref={containerRef}>
            <ReactImgui
                getWasmModule={getWasmModule}
                wasmDataPackage={wasmDataPackage}
                containerRef={containerRef}
            >
                <ImGuiDemo />
            </ReactImgui>
        </div>
    );
}

export default App;

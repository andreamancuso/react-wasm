import { useMemo, useRef } from "react";
import { ReactImgui } from "./lib/components/ReactImgui/components";
// @ts-ignore
import getWasmModule from "./lib/wasm/reactDearImgui.mjs";
// @ts-ignore
import wasmDataPackage from "./lib/wasm/reactDearImgui.data";
import { ImGuiDemo } from "./lib";

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
                getWasmModule={getWasmModule}
                wasmDataPackage={wasmDataPackage}
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

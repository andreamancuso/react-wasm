import { useMemo, useRef } from "react";
import { ReactImgui } from "./lib/components/ReactImgui/components";
// @ts-ignore
import getWasmModule from "./lib/wasm/reactDearImgui.mjs";
// @ts-ignore
import wasmDataPackage from "./lib/wasm/reactDearImgui.data";
// import { ImGuiDemo } from "./lib";
import { GetWasmModule, ImGuiCol } from "./lib/wasm/wasm-app-types";
import { ImGuiStyleForPatching } from "./lib/stylesheet/imgui-style";

import "./App.css";
import { TradingGuiDemo } from "./lib/components/TradingGuiDemo/TradingGuiDemo";
import { theme1, theme2 } from "./lib/stylesheet/themes";

function App() {
    const containerRef = useRef<HTMLDivElement>(null);

    const fontDefs = useMemo(
        () => [
            { name: "roboto-regular", sizes: [12, 14, 16, 18, 24, 48] },
            { name: "roboto-bold", sizes: [12, 14, 16, 18, 24] },
            { name: "roboto-light", sizes: [12, 14, 16, 18, 24] },
            { name: "roboto-mono-regular", sizes: [14, 16] },
        ],
        [],
    );

    const defaultFont = useMemo(() => ({ name: "roboto-regular", size: 16 }), []);

    const styleOverrides: ImGuiStyleForPatching = useMemo(() => theme2, []);

    return (
        <div id="app" ref={containerRef}>
            <ReactImgui
                getWasmModule={getWasmModule as GetWasmModule}
                wasmDataPackage={wasmDataPackage as string}
                containerRef={containerRef}
                fontDefs={fontDefs}
                defaultFont={defaultFont}
                styleOverrides={styleOverrides}
            >
                {/* <ImGuiDemo /> */}
                <TradingGuiDemo />
            </ReactImgui>
        </div>
    );
}

export default App;

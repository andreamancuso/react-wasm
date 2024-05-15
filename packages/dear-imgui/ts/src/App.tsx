import { useRef } from "react";
import { ReactImgui } from "./lib/components/ReactImgui/components";
// @ts-ignore
import getWasmModule from "./lib/wasm/reactDearImgui.mjs";
import { ImGuiDemo } from "./lib";

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

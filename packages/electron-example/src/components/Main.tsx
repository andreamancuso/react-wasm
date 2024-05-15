// eslint-disable-next-line @typescript-eslint/ban-ts-comment
// @ts-ignore abc
import getWasmModule from "@react-wasm/dear-imgui/dist/reactDearImgui.mjs";
import { ReactImgui, ImGuiDemo } from "@react-wasm/dear-imgui";
import { useRef } from "react";

import "./Main.css";

const Main = () => {
    const containerRef = useRef<HTMLDivElement>(null);

    return (
        <div id="app" ref={containerRef}>
            <ReactImgui getWasmModule={getWasmModule} containerRef={containerRef}>
                <ImGuiDemo />
            </ReactImgui>
        </div>
    );
};

export default Main;

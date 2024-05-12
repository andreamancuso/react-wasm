// import * as React from "react";
import { useRef } from "react";
import { ReactImgui } from "./lib/components/ReactImgui/components";
import { ImGuiDemo } from "./components/ImGuiDemo/ImGuiDemo";

import "./App.css";

function App() {
    const containerRef = useRef<HTMLDivElement>(null);

    return (
        <div id="app" ref={containerRef}>
            <ReactImgui containerRef={containerRef}>
                <ImGuiDemo />
            </ReactImgui>
        </div>
    );
}

export default App;

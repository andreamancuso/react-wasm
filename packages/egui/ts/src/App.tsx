import { useRef } from "react";
import { ReactEgui } from "./lib/components/ReactEgui/components";
import * as Module from "./lib/wasm/eframe_template";

import "./App.css";

function App() {
    const containerRef = useRef<HTMLDivElement>(null);

    return (
        <div id="app" ref={containerRef}>
            <ReactEgui Module={Module} containerRef={containerRef}>
                <ReactEgui.Button label="Hello, world!" />
            </ReactEgui>
        </div>
    );
}

export default App;

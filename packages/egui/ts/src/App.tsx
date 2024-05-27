import { useCallback, useMemo, useRef } from "react";
import { ReactEgui } from "./lib/components/ReactEgui/components";
import initWasmModule, {
    set_widget,
    init_egui,
    set_children,
    append_child,
    get_hierarchy,
    append_data_to_table,
} from "./lib/wasm/eframe_template";
import { EguiDemo } from "./lib/components/EguiDemo/EguiDemo";

import "./App.css";

// todo: improve this
const module: any = {
    initWasmModule,
    set_widget,
    set_children,
    init_egui,
    append_child,
    get_hierarchy,
    append_data_to_table,
};

function App() {
    const containerRef = useRef<HTMLDivElement>(null);

    return (
        <div id="app" ref={containerRef}>
            <ReactEgui initWasmModule={module} containerRef={containerRef}>
                <EguiDemo />
            </ReactEgui>
        </div>
    );
}

export default App;

import { useCallback, useRef } from "react";
import { ReactEgui } from "./lib/components/ReactEgui/components";
import initWasmModule, {
    set_widget,
    init_egui,
    set_children,
    append_child,
    get_hierarchy,
    get_widgets,
} from "./lib/wasm/eframe_template";

import "./App.css";

// todo: improve this
const module: any = {
    initWasmModule,
    set_widget,
    set_children,
    init_egui,
    append_child,
    get_hierarchy,
    get_widgets,
};

function App() {
    const containerRef = useRef<HTMLDivElement>(null);

    const handleClick = useCallback(() => {
        console.log("click!");
    }, []);

    const handleInputTextChange = useCallback((event: any) => {
        console.log("changed text:", event.nativeEvent.value);
    }, []);

    return (
        <div id="app" ref={containerRef}>
            <ReactEgui initWasmModule={module} containerRef={containerRef}>
                <ReactEgui.Horizontal>
                    <ReactEgui.InputText
                        onChange={handleInputTextChange}
                        defaultValue="Hello, world!"
                    />
                    <ReactEgui.Button onClick={handleClick} label="Hello, world!" />
                </ReactEgui.Horizontal>
            </ReactEgui>
        </div>
    );
}

export default App;

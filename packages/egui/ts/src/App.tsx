import { useCallback, useMemo, useRef } from "react";
import { ReactEgui } from "./lib/components/ReactEgui/components";
import initWasmModule, {
    set_widget,
    init_egui,
    set_children,
    append_child,
    get_hierarchy,
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
};

function App() {
    const containerRef = useRef<HTMLDivElement>(null);

    const handleClick = useCallback(() => {
        console.log("click!");
    }, []);

    const handleInputTextChange = useCallback((event: any) => {
        console.log("changed text:", event.nativeEvent.value);
    }, []);

    const handleCheckboxCheckedChange = useCallback((event: any) => {
        console.log("changed checked state:", event.nativeEvent.value);
    }, []);

    const handleCollapsingHeaderChange = useCallback((event: any) => {
        console.log("changed collapsing header state:", event.nativeEvent.value);
    }, []);

    const handleRadioButtonGroupChange = useCallback((event: any) => {
        console.log("changed radio button group value:", event.nativeEvent.value);
    }, []);

    const radioButtonGroupOptions = useMemo(
        () => [
            { label: "A", value: "a", tooltipText: "A means a" },
            { label: "B", value: "b", tooltipText: "B means b" },
            { label: "C", value: "c", tooltipText: "C means c" },
        ],
        [],
    );

    return (
        <div id="app" ref={containerRef}>
            <ReactEgui initWasmModule={module} containerRef={containerRef}>
                <ReactEgui.CollapsingHeader
                    onChange={handleCollapsingHeaderChange}
                    label="Click to reveal contents"
                >
                    <ReactEgui.Horizontal>
                        <ReactEgui.InputText
                            onChange={handleInputTextChange}
                            defaultValue="Hello, world!"
                        />
                        <ReactEgui.Checkbox
                            onChange={handleCheckboxCheckedChange}
                            defaultChecked
                            label="Hello, world?"
                            tooltipText="Yes, hello, world!!!"
                        />

                        <ReactEgui.Button onClick={handleClick} label="Hello, world!" />
                    </ReactEgui.Horizontal>
                    <ReactEgui.Horizontal>
                        <ReactEgui.RadioButton label="A" />
                        <ReactEgui.RadioButton label="B" />
                        <ReactEgui.RadioButton label="C" />
                    </ReactEgui.Horizontal>
                    <ReactEgui.Horizontal>
                        <ReactEgui.RadioButtonGroup
                            defaultValue="a"
                            onChange={handleRadioButtonGroupChange}
                            options={radioButtonGroupOptions}
                        />
                    </ReactEgui.Horizontal>
                </ReactEgui.CollapsingHeader>
            </ReactEgui>
        </div>
    );
}

export default App;

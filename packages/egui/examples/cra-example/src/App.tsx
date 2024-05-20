import React, { useCallback, useEffect, useRef, useState } from "react";
import init, { add_widget, get_content } from "eframe_template";

// eslint-disable-next-line import/first
import "./App.css";

function App() {
    const containerRef = useRef<HTMLDivElement>(null);
    const initRef = useRef(false);
    const [wasmInitialised, setWasmInitialised] = useState(false);
    const [content, setContent] = useState("");
    const count = useRef(0);

    useEffect(() => {
        if (!initRef.current) {
            initRef.current = true;

            init().then(() => {
                setWasmInitialised(true);
            });
        }
    }, []);

    const getContent = useCallback(() => {
        setContent(get_content());
    }, []);

    const handleAddButton = useCallback(() => {
        add_widget(JSON.stringify({ id: count.current, type: "Button", label: "Hello, world!" }));

        count.current++;

        getContent();
    }, [getContent]);

    const handleInputTextButton = useCallback(() => {
        add_widget(
            JSON.stringify({ id: count.current, type: "InputText", value: "Hello, world!" }),
        );

        count.current++;

        getContent();
    }, [getContent]);

    return (
        <div id="app" ref={containerRef}>
            {wasmInitialised && (
                <>
                    <div>
                        <button onClick={handleAddButton}>Add Button</button>
                        <button onClick={handleInputTextButton}>Add InputText</button>
                    </div>
                    <div>
                        <pre>{content}</pre>
                    </div>
                </>
            )}
        </div>
    );
}

export default App;

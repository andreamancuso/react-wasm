import React, { useEffect, useRef } from "react";
import init, { init_react_egui, add_widget } from "eframe_template";

// eslint-disable-next-line import/first
import "./App.css";

function App() {
    const containerRef = useRef<HTMLDivElement>(null);
    const initRef = useRef(false);

    useEffect(() => {
        if (!initRef.current) {
            initRef.current = true;

            init().then(() => {
                init_react_egui();

                add_widget(JSON.stringify({ id: 1, type: "Button", label: "Hello, world!" }));
            });
        }
    }, []);

    return (
        <div id="app" ref={containerRef}>
            egui
        </div>
    );
}

export default App;

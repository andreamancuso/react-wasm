import React, { useEffect, useRef } from "react";
import init, { greet } from "eframe_template";

// eslint-disable-next-line import/first
import "./App.css";

function App() {
    const containerRef = useRef<HTMLDivElement>(null);

    useEffect(() => {
        init().then(() => {
            greet("poba");
        });
    }, []);

    return (
        <div id="app" ref={containerRef}>
            egui
        </div>
    );
}

export default App;

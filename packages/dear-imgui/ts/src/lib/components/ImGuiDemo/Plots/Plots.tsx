import React, { useCallback, useRef, useState } from "react";
import { ReactImgui } from "src/lib/components/ReactImgui/components";
import { PlotViewImperativeHandle } from "../../ReactImgui/PlotView";

export const Plots = () => {
    const plotRef = useRef<PlotViewImperativeHandle>(null);
    const [axisAutoFit, setAxisAutoFit] = useState(true);
    const counterRef = useRef(0);

    const appendData = useCallback(() => {
        if (plotRef.current) {
            counterRef.current += 1;
            plotRef.current.appendData(counterRef.current, counterRef.current);
        }
    }, []);

    const resetData = useCallback(() => {
        if (plotRef.current) {
            plotRef.current.resetData();
        }
    }, []);

    const toggleAxisAutoFit = useCallback(() => {
        setAxisAutoFit((val) => !val);
    }, []);

    return (
        <ReactImgui.Node
            style={{
                width: "100%",
                height: "100%",
                flexDirection: "column",
                gap: { row: 5 },
            }}
        >
            <ReactImgui.Node
                style={{
                    width: "100%",
                    flexDirection: "row",
                    alignItems: "center",
                    gap: { column: 15 },
                }}
            >
                <ReactImgui.Button onClick={appendData} label="Append" />
                <ReactImgui.Button onClick={toggleAxisAutoFit} label="Toggle Axis Auto-Fit" />
                <ReactImgui.Button onClick={resetData} label="Reset" />
            </ReactImgui.Node>

            <ReactImgui.PlotView ref={plotRef} style={{ flex: 1 }} axisAutoFit={axisAutoFit} />
        </ReactImgui.Node>
    );
};

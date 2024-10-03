import React, {
    LegacyRef,
    MutableRefObject,
    RefObject,
    useCallback,
    useEffect,
    useMemo,
    useRef,
    useState,
} from "react";
import { subMinutes } from "date-fns";
import { ReactImgui } from "src/lib/components/ReactImgui/components";
import { PlotViewImperativeHandle } from "../../ReactImgui/PlotView";
import { ImPlotScale } from "src/lib/wasm/wasm-app-types";
import { useStore } from "../store";

type PlotRefs = {
    [key: string]: RefObject<PlotViewImperativeHandle> | undefined;
};

type Props = {};

export const Plots = ({}: Props) => {
    const symbols = useStore((state) => state.symbols);

    const plotRefs: PlotRefs = symbols.reduce((acc, item) => {
        acc[item] = useRef<PlotViewImperativeHandle>(null);

        return acc;
    }, {} as any);
    const [axisAutoFit, setAxisAutoFit] = useState(true);

    // const appendData = useCallback(() => {
    //     if (plotRef.current) {
    //         counterRef.current += 1;
    //         plotRef.current.appendData(counterRef.current, counterRef.current);
    //     }
    // }, []);

    // const resetData = useCallback(() => {
    //     if (plotRef.current) {
    //         plotRef.current.resetData();
    //     }
    // }, []);

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
                <ReactImgui.Button onClick={toggleAxisAutoFit} label="Toggle Axis Auto-Fit" />
            </ReactImgui.Node>

            {symbols.map((sym) => (
                <>
                    <ReactImgui.UnformattedText text={sym} key={`${sym}-label`} />
                    <ReactImgui.PlotView
                        key={`${sym}-plot`}
                        xAxisScale={ImPlotScale.Time}
                        ref={plotRefs[sym]}
                        style={{ width: "100%", height: 400 }}
                        axisAutoFit={axisAutoFit}
                    />
                </>
            ))}
        </ReactImgui.Node>
    );
};

import React from "react";
import { ReactImgui } from "src/lib/components/ReactImgui/components";
import { useStore } from "../store";
import { CryptoLinePlot } from "./CryptoLinePlot";

type Props = {};

export const CryptoPlots = ({}: Props) => {
    const symbols = useStore((state) => state.symbols);

    return (
        <ReactImgui.Node
            style={{
                width: "100%",
                height: "100%",
                flexDirection: "column",
                gap: { row: 5 },
            }}
        >
            {symbols.map((sym) => (
                <CryptoLinePlot key={sym} symbol={sym} />
            ))}
        </ReactImgui.Node>
    );
};

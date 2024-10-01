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
import { ReactImgui } from "src/lib/components/ReactImgui/components";
import { PlotViewImperativeHandle } from "../../ReactImgui/PlotView";
import { ImPlotScale } from "src/lib/wasm/wasm-app-types";

type PlotRefs = {
    [key: string]: RefObject<PlotViewImperativeHandle> | undefined;
};

const symbols = [
    "SUSHI/USD",
    "YFI/USD",
    "LTC/USD",
    "DOGE/USD",
    "BCH/USDT",
    "BAT/USDC",
    "BAT/USD",
    "CRV/USDC",
    "SHIB/USDT",
    "BCH/USDC",
    "LINK/USDT",
    "LINK/USDC",
    "BCH/USD",
    "CRV/USD",
    "AVAX/USDT",
    "LTC/USDT",
    "XTZ/USDC",
    "BTC/USDT",
    "AVAX/USDC",
    "XTZ/USD",
    // "AVAX/USD",
    // "LTC/USDC",
    // "LINK/USD",
    // "LINK/BTC",
    // "SHIB/USDC",
    // "SOL/USDC",
    // "GRT/USDC",
    // "USDT/USDC",
    // "SOL/USD",
    // "SOL/USDT",
    // "SHIB/USD",
    // "DOGE/USDC",
    // "GRT/USD",
    // "ETH/USDT",
    // "USDT/USD",
    // "ETH/USDC",
    // "LTC/BTC",
    // "AAVE/USDT",
    // "ETH/USD",
    // "UNI/USDT",
    // "ETH/BTC",
    // "BCH/BTC",
    // "USDC/USD",
    // "YFI/USDT",
    // "UNI/USDC",
    // "DOT/USDC",
    // "DOT/USD",
    // "AAVE/USD",
    // "MKR/USDC",
    // "UNI/USD",
    // "AAVE/USDC",
    // "UNI/BTC",
    // "SUSHI/USDT",
    // "BTC/USDC",
    // "BTC/USD",
    // "SUSHI/USDC",
    // "YFI/USDC",
    // "MKR/USD",
    // "DOGE/USDT",
];

export const Plots = () => {
    // const symbols = useMemo(() => ["BTC/USD", "ETH/USD", "XTZ/USDC"], []);
    const socketRef = useRef<WebSocket>();
    const plotRefs: PlotRefs = symbols.reduce((acc, item) => {
        acc[item] = useRef<PlotViewImperativeHandle>(null);

        return acc;
    }, {} as any);
    // {
    //     "BTC/USD": useRef<PlotViewImperativeHandle>(null),
    //     "ETH/USD": useRef<PlotViewImperativeHandle>(null),
    //     "XTZ/USDC": useRef<PlotViewImperativeHandle>(null),
    // };
    const [axisAutoFit, setAxisAutoFit] = useState(true);
    const counterRef = useRef(0);

    const connect = useCallback(() => {
        socketRef.current = new WebSocket("ws://localhost:4000");

        socketRef.current.addEventListener("error", () => {
            console.log("Connected to server");
        });

        socketRef.current.addEventListener("open", () => {
            console.log("Connected to server");
        });

        socketRef.current.addEventListener("close", (ev) => {
            try {
                const reason = JSON.parse(ev.reason);

                console.log(`Disconnected from server: ${reason.message}`);
            } catch (error) {
                console.log("Disconnected from server");
            }
        });

        socketRef.current.addEventListener("message", (event) => {
            const data = JSON.parse(event.data);

            console.log(data);

            if (Array.isArray(data)) {
                console.log(data.map(({ symbol }) => symbol));
            } else {
                const ref = plotRefs[data.S];

                if (ref && ref.current) {
                    const timestamp = Math.floor(Number(new Date(data.Timestamp)) / 1000);

                    console.log(timestamp);

                    ref.current.appendData(timestamp, data.BidPrice);
                }
            }
        });
    }, []);

    const subscribeToLiveData = useCallback(() => {
        // BTC/USD,ETH/USD
        if (socketRef.current) {
            const message = JSON.stringify({
                passkey: "",
                action: "subscribe",
                ticks: symbols,
            });

            console.log(message);

            socketRef.current.send(message);
        }
    }, [symbols]);

    const getAssets = useCallback(() => {
        if (socketRef.current) {
            const message = JSON.stringify({
                passkey: "",
                action: "getAssets",
            });

            socketRef.current.send(message);
        }
    }, [symbols]);

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
                <ReactImgui.Button onClick={connect} label="Connect" />
                <ReactImgui.Button onClick={subscribeToLiveData} label="Subscribe to live data" />
                <ReactImgui.Button onClick={getAssets} label="Get Assets" />
                {/* <ReactImgui.Button onClick={appendData} label="Append" /> */}
                <ReactImgui.Button onClick={toggleAxisAutoFit} label="Toggle Axis Auto-Fit" />
                {/* <ReactImgui.Button onClick={resetData} label="Reset" /> */}
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

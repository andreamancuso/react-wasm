import React, { useCallback, useEffect, useMemo, useRef, useState } from "react";
import { subDays, subHours, subMinutes } from "date-fns";
import { Subscribe } from "@react-rxjs/core";
import {
    GetCryptoBarsParams,
    GetQuotesParams,
} from "@alpacahq/alpaca-trade-api/dist/resources/datav2/rest_v2";
import { ReactImgui } from "src/lib/components/ReactImgui/components";
import { useWidgetRegistrationService } from "src/lib/hooks";
import { ImGuiCol, ImGuiStyleVar } from "src/lib/wasm/wasm-app-types";
// import { HelpMarker } from "./HelpMarker/HelpMarker";
import faIconMap from "../../fa-icons";
import RWStyleSheet from "../../stylesheet/stylesheet";
import { CryptoLinePlots } from "./CryptoPlots/CryptoLinePlots";
import { TreeViewItem } from "../ReactImgui/TreeView";
import { useStore } from "./store";
import { CryptoAssetsList } from "./CryptoAssetsList/CryptoAssetsList";
import { DataService } from "./dataService";
import { DataServiceContext } from "./dataServiceContext";
import { cryptoSymbols } from "./cryptoSymbols";
import { CryptoAssetPanels } from "./CryptoAssetPanels/CryptoAssetPanels";
import { theme2Colors } from "src/lib/stylesheet/themes";
import { CryptoCandlestickPlots } from "./CryptoPlots/CryptoCandlestickPlots";

const componentMap = {
    cryptoAssetPanels: CryptoAssetPanels,
    cryptoAssetList: CryptoAssetsList,
    cryptoLinePlots: CryptoLinePlots,
    cryptoCandlestickPlots: CryptoCandlestickPlots,
};

const dataService = new DataService();

type ComponentKeys = keyof typeof componentMap;

export const TradingGuiDemo = () => {
    const symbols = useStore((state) => state.symbols);
    const setCryptoAssets = useStore((state) => state.setCryptoAssets);

    const socketRef = useRef<WebSocket>();
    const widgetRegistratonService = useWidgetRegistrationService();

    const [selectedItemIds, setSelectedItemIds] = useState<ComponentKeys[]>(["cryptoAssetPanels"]);

    const treeViewItems: TreeViewItem[] = useMemo(() => {
        return [
            {
                itemId: "cryptoAssetPanels",
                label: "Crypto Asset Panels",
            },
            {
                itemId: "cryptoAssetList",
                label: "Crypto Asset List",
            },
            {
                itemId: "cryptoLinePlots",
                label: "Crypto Line Plots",
            },
            {
                itemId: "cryptoCandlestickPlots",
                label: "Crypto Candlestick Plots",
            },
        ];
    }, []);

    const styleSheet = useMemo(
        () =>
            RWStyleSheet.create({
                rootNode: {
                    height: "100%",
                    // padding: {
                    //     all: 10,
                    // },
                    // gap: { row: 12 },
                },
                mainLayoutNode: {
                    flex: 1,
                    flexDirection: "row",
                    // gap: { column: 12 },
                },
                sidebarNode: {
                    flexBasis: 60,
                    height: "100%",
                    borderColor: "#1B1D20",
                    borderThickness: 1,
                },
                sideBarItem: {
                    width: 58,
                    height: 58,
                    justifyContent: "center",
                    alignItems: "center",
                },
                icon: {
                    width: 48,
                    height: 48,
                    font: { name: "roboto-regular", size: 36 },
                    colors: {
                        [ImGuiCol.ButtonHovered]: "#001033",
                        [ImGuiCol.ButtonActive]: "#001033",
                    },
                    vars: { [ImGuiStyleVar.FrameRounding]: 24 },
                },
                iconActive: {
                    width: 48,
                    height: 48,
                    font: { name: "roboto-regular", size: 36 },
                    vars: { [ImGuiStyleVar.FrameRounding]: 24 },
                    colors: {
                        [ImGuiCol.Text]: "#588AF5",
                        [ImGuiCol.Button]: "#001033",
                        [ImGuiCol.ButtonHovered]: "#001033",
                        [ImGuiCol.ButtonActive]: "#001033",
                    },
                },
                // contentNode: {
                //     flex: 1,
                //     height: "100%",
                //     borderColor: "#000",
                //     borderThickness: 1,
                //     padding: { all: 5 },
                // },
                title: {
                    colors: { [ImGuiCol.Text]: theme2Colors.green },
                    font: { name: "roboto-regular", size: 24 },
                },
                debugButton: {
                    positionType: "absolute",
                    position: { right: 15, bottom: 15 },
                    flexDirection: "row",
                    gap: { column: 10 },
                },
                button: {
                    colors: {
                        [ImGuiCol.Text]: theme2Colors.green,
                        [ImGuiCol.Border]: theme2Colors.green,
                    },
                    // borderColor: theme2Colors.green,
                    // borderThickness: 1,
                    padding: { all: 8 },
                    vars: {
                        [ImGuiStyleVar.FrameBorderSize]: 1,
                        [ImGuiStyleVar.FrameRounding]: 5,
                    },
                },
            }),
        [],
    );

    // todo: move to dataService.ts ?
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

            if (data.cryptoAssets) {
                // todo: remove filtering
                setCryptoAssets(
                    data.cryptoAssets.filter((asset: any) => cryptoSymbols.includes(asset.symbol)),
                );
            } else if (data.cryptoQuote) {
                dataService.addCryptoQuote(data.cryptoQuote);
            } else if (data.latestCryptoQuotes) {
                // console.log(data.latestCryptoQuotes);

                Object.entries(data.latestCryptoQuotes).forEach(([symbol, cryptoQuote]) =>
                    dataService.addCryptoQuote({ ...(cryptoQuote as any), S: symbol }),
                );
            } else if (data.cryptoSnapshots) {
                dataService.addCryptoSnapshots(data.cryptoSnapshots);
                // console.log(data.cryptoSnapshots);
            } else if (data.latestCryptoBars) {
                console.log(data.latestCryptoBars);
            } else if (data.cryptoBars) {
                dataService.addCryptoBars(data.cryptoBars);
                // console.log(data.cryptoBars);
            }
        });
    }, [setCryptoAssets]);

    const sendMessage = useCallback((message: any) => {
        if (socketRef.current) {
            socketRef.current.send(JSON.stringify(message));
        }
    }, []);

    const subscribeToLiveData = useCallback(
        () =>
            sendMessage({
                passkey: "",
                action: "subscribeForCryptoQuotes",
                symbols,
            }),
        [sendMessage, symbols],
    );

    const getLatestQuotes = useCallback(() => {
        const currentDate = new Date();
        const end = currentDate.toISOString();
        const start = subMinutes(currentDate, 1);

        sendMessage({
            action: "getQuotes",
            symbols,
            options: { start, end },
        });
    }, [symbols]);

    const getCryptoQuotes = useCallback(() => {
        const currentDate = new Date();
        const end = currentDate.toISOString();
        const start = subMinutes(currentDate, 1).toISOString();
        const options: GetQuotesParams = {
            start,
            end,
        };

        sendMessage({
            action: "getCryptoQuotes",
            symbols,
            options,
        });
    }, [symbols]);

    const getCryptoBars = useCallback(() => {
        const currentDate = new Date();
        const end = currentDate.toISOString();
        const start = subDays(currentDate, 30).toISOString();
        const options: GetCryptoBarsParams = { start, end, timeframe: "1D" };

        sendMessage({
            action: "getCryptoBars",
            symbols,
            options,
        });
    }, [symbols]);

    const getCryptoSnapshots = useCallback(
        () =>
            sendMessage({
                action: "getCryptoSnapshots",
                symbols,
            }),
        [symbols],
    );

    const getLatestCryptoQuotes = useCallback(
        () =>
            sendMessage({
                action: "getLatestCryptoQuotes",
                symbols,
            }),
        [symbols],
    );

    const getLatestCryptoBars = useCallback(
        () =>
            sendMessage({
                action: "getLatestCryptoBars",
                symbols,
            }),
        [symbols],
    );

    const getCryptoAssets = useCallback(
        () =>
            sendMessage({
                action: "getCryptoAssets",
                symbols,
            }),
        [symbols],
    );

    const debugModeBtnClicked = useCallback(() => {
        widgetRegistratonService.setDebug(true);
    }, []);

    const onToggleItemSelection = useCallback((itemId: string, selected: boolean) => {
        setSelectedItemIds((selection) => {
            if (selected) {
                return [itemId as ComponentKeys];
            } else {
                return selection.filter((item) => item !== itemId);
            }
        });
    }, []);

    useEffect(() => {
        // connect();
        // setTimeout(() => {
        //     getCryptoAssets();
        // }, 1000);
    }, [connect, getCryptoAssets]);

    const Component = componentMap[selectedItemIds[0]];

    return (
        <ReactImgui.Node root style={styleSheet.rootNode}>
            {/* <ReactImgui.UnformattedText text="Trading GUI demo" style={styleSheet.title} /> */}

            {/* <ReactImgui.Node
                style={{
                    width: "100%",
                    flexDirection: "row",
                    alignItems: "center",
                    gap: { column: 15 },
                }}
            >
                <ReactImgui.Button onClick={connect} label="Connect" style={styleSheet.button} />
                <ReactImgui.Button
                    onClick={subscribeToLiveData}
                    label="Subscribe to live data"
                    style={styleSheet.button}
                />
                <ReactImgui.Button
                    onClick={getCryptoAssets}
                    label="Get Crypto Assets"
                    style={styleSheet.button}
                />
                <ReactImgui.Button
                    onClick={getCryptoQuotes}
                    label="Get Crypto Quotes"
                    style={styleSheet.button}
                />
                <ReactImgui.Button
                    onClick={getCryptoSnapshots}
                    label="Get Crypto Snapshots"
                    style={styleSheet.button}
                />
                <ReactImgui.Button
                    onClick={getCryptoBars}
                    label="Get Crypto Bars"
                    style={styleSheet.button}
                />
                <ReactImgui.Button
                    onClick={getLatestCryptoQuotes}
                    label="Get Latest Crypto Quotes"
                    style={styleSheet.button}
                />
                <ReactImgui.Button
                    onClick={getLatestCryptoBars}
                    label="Get Latest Crypto Bars"
                    style={styleSheet.button}
                />
            </ReactImgui.Node> */}

            <ReactImgui.Node style={styleSheet.mainLayoutNode}>
                <ReactImgui.Node style={styleSheet.sidebarNode}>
                    {/* <ReactImgui.TreeView
                        items={treeViewItems}
                        selectedItemIds={selectedItemIds}
                        onToggleItemSelection={onToggleItemSelection}
                    /> */}
                    <ReactImgui.Node style={styleSheet.sideBarItem}>
                        <ReactImgui.UnformattedText
                            text={faIconMap.otter}
                            style={styleSheet.icon}
                        />
                    </ReactImgui.Node>
                    {/* <ReactImgui.Node style={styleSheet.sideBarItem}>
                        <ReactImgui.UnformattedText
                            text={faIconMap["cart-plus"]}
                            style={styleSheet.icon}
                        />
                    </ReactImgui.Node>
                    <ReactImgui.Node style={styleSheet.sideBarItem}>
                        <ReactImgui.UnformattedText
                            text={faIconMap["wallet"]}
                            style={styleSheet.icon}
                        />
                    </ReactImgui.Node>
                    <ReactImgui.Node style={styleSheet.sideBarItem}>
                        <ReactImgui.UnformattedText
                            text={faIconMap["coins"]}
                            style={styleSheet.icon}
                        />
                    </ReactImgui.Node>
                    <ReactImgui.Node style={styleSheet.sideBarItem}>
                        <ReactImgui.UnformattedText
                            text={faIconMap["dollar-sign"]}
                            style={styleSheet.icon}
                        />
                    </ReactImgui.Node>
                    <ReactImgui.Node style={styleSheet.sideBarItem}>
                        <ReactImgui.UnformattedText
                            text={faIconMap["wave-square"]}
                            style={styleSheet.icon}
                        />
                    </ReactImgui.Node>
                    <ReactImgui.Node style={styleSheet.sideBarItem}>
                        <ReactImgui.UnformattedText
                            text={faIconMap["stairs"]}
                            style={styleSheet.icon}
                        />
                    </ReactImgui.Node>
                    <ReactImgui.Node style={styleSheet.sideBarItem}>
                        <ReactImgui.UnformattedText
                            text={faIconMap["square-poll-vertical"]}
                            style={styleSheet.icon}
                        />
                    </ReactImgui.Node>
                    <ReactImgui.Node style={styleSheet.sideBarItem}>
                        <ReactImgui.UnformattedText
                            text={faIconMap["chart-pie"]}
                            style={styleSheet.icon}
                        />
                    </ReactImgui.Node>
                    <ReactImgui.Node style={styleSheet.sideBarItem}>
                        <ReactImgui.UnformattedText
                            text={faIconMap["chart-line"]}
                            style={styleSheet.icon}
                        />
                    </ReactImgui.Node>
                    <ReactImgui.Node style={styleSheet.sideBarItem}>
                        <ReactImgui.UnformattedText
                            text={faIconMap["chart-column"]}
                            style={styleSheet.icon}
                        />
                    </ReactImgui.Node>
                    <ReactImgui.Node style={styleSheet.sideBarItem}>
                        <ReactImgui.UnformattedText
                            text={faIconMap["chart-bar"]}
                            style={styleSheet.icon}
                        />
                    </ReactImgui.Node>
                    <ReactImgui.Node style={styleSheet.sideBarItem}>
                        <ReactImgui.UnformattedText
                            text={faIconMap["chart-area"]}
                            style={styleSheet.icon}
                        />
                    </ReactImgui.Node> */}
                    <ReactImgui.Node style={styleSheet.sideBarItem}>
                        <ReactImgui.Button
                            label={faIconMap["arrow-trend-up"]}
                            style={styleSheet.icon}
                            hoverStyle={styleSheet.iconActive}
                        />
                    </ReactImgui.Node>
                    <ReactImgui.Node style={styleSheet.sideBarItem}>
                        <ReactImgui.Button
                            label={faIconMap["arrow-trend-up"]}
                            style={styleSheet.iconActive}
                        />
                    </ReactImgui.Node>
                    {/* <ReactImgui.Node style={styleSheet.sideBarItem}>
                        <ReactImgui.UnformattedText
                            text={faIconMap["arrow-trend-down"]}
                            style={styleSheet.icon}
                        />
                    </ReactImgui.Node> */}
                </ReactImgui.Node>
                {/* <ReactImgui.Node style={styleSheet.contentNode} cull={false}>
                    {Component && (
                        <DataServiceContext.Provider value={dataService}>
                            <Component />
                        </DataServiceContext.Provider>
                    )}
                </ReactImgui.Node> */}
            </ReactImgui.Node>

            <ReactImgui.Node style={styleSheet.debugButton}>
                <ReactImgui.Button label={faIconMap.bug} onClick={debugModeBtnClicked} />
            </ReactImgui.Node>
        </ReactImgui.Node>
    );
};

import React, { useCallback, useEffect, useMemo, useRef, useState } from "react";
import { ReactImgui } from "src/lib/components/ReactImgui/components";
import RWStyleSheet from "../../../stylesheet/stylesheet";
import { theme2Colors } from "src/lib/stylesheet/themes";

type Props = {};

// todo: fix 1px vertical shift on hover
export const Tabs = ({}: Props) => {
    const [selectedItemIds, setSelectedItemIds] = useState("cryptoAssetPanels");

    const styleSheet = useMemo(
        () =>
            RWStyleSheet.create({
                tabs: {
                    flexDirection: "row",
                    borderLeft: {
                        color: "#1C1E22",
                        thickness: 1,
                    },
                    borderBottom: {
                        color: "#1C1E22",
                        thickness: 1,
                    },
                },
                tab: {
                    padding: { vertical: 10, horizontal: 20 },
                    borderTop: {
                        color: "#1C1E22",
                        thickness: 1,
                    },
                    borderRight: {
                        color: "#1C1E22",
                        thickness: 1,
                    },
                },
                hoveredTab: {
                    padding: { vertical: 10, horizontal: 20 },
                    borderTop: {
                        // todo: not currently working for hover/active/disabled states
                        color: "#fff",
                        thickness: 1,
                    },
                    borderRight: {
                        color: "#1C1E22",
                        thickness: 1,
                    },
                    position: {
                        top: 1, // todo: hack
                    },
                },
                activeTab: {
                    padding: { vertical: 10, horizontal: 20 },
                    borderTop: {
                        // todo: not currently working for hover/active/disabled states
                        color: "#fff",
                        thickness: 1,
                    },
                    borderRight: {
                        color: "#1C1E22",
                        thickness: 1,
                    },
                    position: {
                        top: 1, // todo: hack
                    },
                },
                selectedTab: {
                    padding: { vertical: 10, horizontal: 20 },
                    borderTop: {
                        color: "#fff",
                        thickness: 1,
                    },
                    borderBottom: {
                        color: theme2Colors.black,
                        thickness: 1,
                    },
                    borderRight: {
                        color: "#1C1E22",
                        thickness: 1,
                    },
                    position: {
                        top: 1, // todo: hack
                    },
                },
                tabText: {
                    font: { name: "roboto-bold", size: 18 },
                },
            }),
        [],
    );

    return (
        <ReactImgui.Node style={styleSheet.tabs}>
            <ReactImgui.Node style={styleSheet.selectedTab}>
                <ReactImgui.UnformattedText text="Markets" style={styleSheet.tabText} />
            </ReactImgui.Node>
            <ReactImgui.Node
                style={styleSheet.tab}
                hoverStyle={styleSheet.hoveredTab}
                activeStyle={styleSheet.activeTab}
                onClick={() => {
                    console.log("clicked");
                }}
                trackMouseClickEvents
            >
                <ReactImgui.UnformattedText text="History" style={styleSheet.tabText} />
            </ReactImgui.Node>
        </ReactImgui.Node>
    );
};

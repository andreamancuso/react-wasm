import React, { useCallback, useEffect, useMemo, useRef, useState } from "react";
import { ReactImgui } from "../../ReactImgui";
import RWStyleSheet from "../../../src/lib/stylesheet/stylesheet";
import { theme2Colors } from "../../../src/lib/stylesheet/themes";

type Props = {
    tabs: string[];
    selectedTabIndex: number;
    onSelectedTabChange: (index: number) => void;
};

// todo: fix 1px vertical shift on hover
export const Tabs = ({ tabs, selectedTabIndex, onSelectedTabChange }: Props) => {
    // const [selectedItemIds, setSelectedItemIds] = useState("cryptoAssetPanels");

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
            {tabs.map((tab, index) => {
                const isSelected = selectedTabIndex === index;

                return (
                    <ReactImgui.Node
                        key={tab}
                        style={isSelected ? styleSheet.selectedTab : styleSheet.tab}
                        hoverStyle={isSelected ? styleSheet.selectedTab : styleSheet.hoveredTab}
                        activeStyle={isSelected ? styleSheet.selectedTab : styleSheet.activeTab}
                        onClick={() => {
                            // console.log("clicked");
                            onSelectedTabChange(index);
                        }}
                        trackMouseClickEvents
                    >
                        <ReactImgui.UnformattedText text={tab} style={styleSheet.tabText} />
                    </ReactImgui.Node>
                );
            })}
        </ReactImgui.Node>
    );
};

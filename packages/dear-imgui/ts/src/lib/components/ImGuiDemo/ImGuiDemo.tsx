import React, { useCallback, useMemo, useState } from "react";
import { ReactImgui } from "src/lib/components/ReactImgui/components";
import { useWidgetRegistrationService } from "src/lib/hooks";
import { ImGuiCol, ImGuiStyleVar } from "src/lib/wasm/wasm-app-types";
// import { HelpMarker } from "./HelpMarker/HelpMarker";
import faIconMap from "../../fa-icons";
import RWStyleSheet from "../../stylesheet/stylesheet";
import { Tables } from "./Tables/Tables";
import { Maps } from "./Maps/Maps";
import { Plots } from "./Plots/Plots";
import { TreeViewItem } from "../ReactImgui/TreeView";
import { Images } from "./Images/Images";
import { Icons } from "./Icons/Icons";
import { TextFields } from "./TextFields/TextFields";
import { ClippedMultiLineTextRenderers } from "./ClippedMultiLineTextRenderers/ClippedMultiLineTextRenderers";
import { Sliders } from "./Sliders/Sliders";

const componentMap = {
    textField: TextFields,
    icons: Icons,
    images: Images,
    sliders: Sliders,
    maps: Maps,
    plots: Plots,
    tables: Tables,
    clippedMultiLineTextRenderers: ClippedMultiLineTextRenderers,
};

type ComponentKeys = keyof typeof componentMap;

export const ImGuiDemo = () => {
    const widgetRegistratonService = useWidgetRegistrationService();

    const [selectedItemIds, setSelectedItemIds] = useState<ComponentKeys[]>(["plots"]);

    const treeViewItems: TreeViewItem[] = useMemo(() => {
        return [
            {
                itemId: "textField",
                label: "Text Field",
            },
            {
                itemId: "icons",
                label: "Icons",
            },
            {
                itemId: "images",
                label: "Images",
            },
            {
                itemId: "sliders",
                label: "Sliders",
            },
            {
                itemId: "maps",
                label: "Maps",
            },
            {
                itemId: "plots",
                label: "Plots",
            },
            {
                itemId: "tables",
                label: "Tables",
            },
            {
                itemId: "clippedMultiLineTextRenderers",
                label: "ClippedMultiLineTextRenderers",
            },
        ];
    }, []);

    const styleSheet = useMemo(
        () =>
            RWStyleSheet.create({
                rootNode: {
                    height: "100%",
                    padding: {
                        all: 10,
                    },
                    gap: { row: 12 },
                },
                mainLayoutNode: {
                    flex: 1,
                    flexDirection: "row",
                    gap: { column: 12 },
                },
                sidebarNode: {
                    flexBasis: 200,
                    height: "100%",
                    border: {
                        color: "#000",
                        thickness: 1,
                    },
                },
                contentNode: {
                    flex: 1,
                    height: "100%",
                    border: {
                        color: "#000",
                        thickness: 1,
                    },
                    padding: { all: 5 },
                },
                title: {
                    colors: { [ImGuiCol.Text]: "#ff6e59" },
                    font: { name: "roboto-regular", size: 24 },
                },
                debugButton: {
                    positionType: "absolute",
                    position: { right: 15, bottom: 15 },
                    flexDirection: "row",
                    gap: { column: 10 },
                },
            }),
        [],
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

    const Component = componentMap[selectedItemIds[0]];

    return (
        <ReactImgui.Node root style={styleSheet.rootNode}>
            <ReactImgui.UnformattedText text="React Dear Imgui bindings" style={styleSheet.title} />

            <ReactImgui.Node style={styleSheet.mainLayoutNode}>
                <ReactImgui.Node style={styleSheet.sidebarNode}>
                    <ReactImgui.TreeView
                        items={treeViewItems}
                        selectedItemIds={selectedItemIds}
                        onToggleItemSelection={onToggleItemSelection}
                    />
                </ReactImgui.Node>
                <ReactImgui.Node style={styleSheet.contentNode} cull>
                    {Component && <Component />}
                </ReactImgui.Node>
            </ReactImgui.Node>

            <ReactImgui.Node style={styleSheet.debugButton}>
                <ReactImgui.Button label={faIconMap.bug} onClick={debugModeBtnClicked} />
            </ReactImgui.Node>
        </ReactImgui.Node>
    );
};

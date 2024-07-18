import { useEffect, useMemo, useRef } from "react";
// @ts-ignore
import Pbf from "pbf";
// @ts-ignore
import { VectorTile } from "@mapbox/vector-tile";

import osmtogeojson from "osmtogeojson";

import { ReactImgui } from "./lib/components/ReactImgui/components";
// @ts-ignore
import getWasmModule from "./lib/wasm/reactDearImgui.mjs";
// @ts-ignore
import wasmDataPackage from "./lib/wasm/reactDearImgui.data";
import { ImGuiDemo } from "./lib";
import { GetWasmModule, ImGuiCol } from "./lib/wasm/wasm-app-types";
import { ImGuiStyleForPatching } from "./lib/stylesheet/imgui-style";

import "./App.css";

export const themeColors = {
    black: "1a1a1a",
    darkGrey: "5a5a5a",
    grey: "9a9a9a",
    lightGrey: "bebebe",
    veryLightGrey: "e5e5e5",
    superLightGrey: "f7f7f7",
    white: "ffffff",
    hero: "ff6e59",
    hoverHero: "ff4a30",
};

function App() {
    const containerRef = useRef<HTMLDivElement>(null);

    const fontDefs = useMemo(
        () => [
            { name: "roboto-regular", sizes: [12, 14, 16, 18, 24] },
            { name: "roboto-bold", sizes: [12, 14, 16, 18, 24] },
            { name: "roboto-light", sizes: [12, 14, 16, 18, 24] },
            { name: "roboto-mono-regular", sizes: [14] },
        ],
        [],
    );

    const defaultFont = useMemo(() => ({ name: "roboto-regular", size: 16 }), []);

    const styleOverrides: ImGuiStyleForPatching = useMemo(
        () => ({
            // frameBorderSize: 1,
            // windowPadding: [20, 20],
            colors: {
                [ImGuiCol.Text]: [themeColors.black, 1],
                [ImGuiCol.TextDisabled]: [themeColors.darkGrey, 1],
                [ImGuiCol.WindowBg]: [themeColors.white, 1],
                [ImGuiCol.ChildBg]: [themeColors.white, 1],
                [ImGuiCol.PopupBg]: [themeColors.white, 1],
                [ImGuiCol.Border]: [themeColors.darkGrey, 1],
                [ImGuiCol.BorderShadow]: [themeColors.black, 1],
                [ImGuiCol.FrameBg]: [themeColors.veryLightGrey, 1],
                [ImGuiCol.FrameBgHovered]: [themeColors.lightGrey, 1],
                [ImGuiCol.FrameBgActive]: [themeColors.grey, 1],
                [ImGuiCol.TitleBg]: [themeColors.lightGrey, 1],
                [ImGuiCol.TitleBgActive]: [themeColors.grey, 1],
                [ImGuiCol.TitleBgCollapsed]: [themeColors.veryLightGrey, 1],
                [ImGuiCol.MenuBarBg]: [themeColors.grey, 1],
                [ImGuiCol.ScrollbarBg]: [themeColors.veryLightGrey, 1],
                [ImGuiCol.ScrollbarGrab]: [themeColors.grey, 1],
                [ImGuiCol.ScrollbarGrabHovered]: [themeColors.darkGrey, 1],
                [ImGuiCol.ScrollbarGrabActive]: [themeColors.black, 1],
                [ImGuiCol.CheckMark]: [themeColors.black, 1],
                [ImGuiCol.SliderGrab]: [themeColors.grey, 1],
                [ImGuiCol.SliderGrabActive]: [themeColors.darkGrey, 1],
                [ImGuiCol.Button]: [themeColors.lightGrey, 1],
                [ImGuiCol.ButtonHovered]: [themeColors.grey, 1],
                [ImGuiCol.ButtonActive]: [themeColors.darkGrey, 1],
                [ImGuiCol.Header]: [themeColors.veryLightGrey, 1],
                [ImGuiCol.HeaderHovered]: [themeColors.lightGrey, 1],
                [ImGuiCol.HeaderActive]: [themeColors.grey, 1],
                [ImGuiCol.Separator]: [themeColors.superLightGrey, 1],
                [ImGuiCol.SeparatorHovered]: [themeColors.veryLightGrey, 1],
                [ImGuiCol.SeparatorActive]: [themeColors.lightGrey, 1],
                [ImGuiCol.ResizeGrip]: [themeColors.veryLightGrey, 1],
                [ImGuiCol.ResizeGripHovered]: [themeColors.lightGrey, 1],
                [ImGuiCol.ResizeGripActive]: [themeColors.grey, 1],
                [ImGuiCol.Tab]: [themeColors.veryLightGrey, 1],
                [ImGuiCol.TabHovered]: [themeColors.lightGrey, 1],
                [ImGuiCol.TabActive]: [themeColors.grey, 1],
                [ImGuiCol.TabUnfocused]: [themeColors.veryLightGrey, 1],
                [ImGuiCol.TabUnfocusedActive]: [themeColors.lightGrey, 1],
                [ImGuiCol.PlotLines]: [themeColors.grey, 1],
                [ImGuiCol.PlotLinesHovered]: [themeColors.darkGrey, 1],
                [ImGuiCol.PlotHistogram]: [themeColors.grey, 1],
                [ImGuiCol.PlotHistogramHovered]: [themeColors.darkGrey, 1],
                [ImGuiCol.TableHeaderBg]: [themeColors.grey, 1],
                [ImGuiCol.TableBorderStrong]: [themeColors.darkGrey, 1],
                [ImGuiCol.TableBorderLight]: [themeColors.lightGrey, 1],
                [ImGuiCol.TableRowBg]: [themeColors.veryLightGrey, 1],
                [ImGuiCol.TableRowBgAlt]: [themeColors.white, 1],
                [ImGuiCol.TextSelectedBg]: [themeColors.grey, 1],
                [ImGuiCol.DragDropTarget]: [themeColors.grey, 1],
                [ImGuiCol.NavHighlight]: [themeColors.grey, 1],
                [ImGuiCol.NavWindowingHighlight]: [themeColors.grey, 1],
                [ImGuiCol.NavWindowingDimBg]: [themeColors.grey, 1],
                [ImGuiCol.ModalWindowDimBg]: [themeColors.grey, 1],
            },
        }),
        [],
    );

    useEffect(() => {
        const retrievePbfAsset = async () => {
            const response = await fetch("/assets/test.pbf");
            const pbfBlob = await response.blob();
            const pbfArrayBuffer = await pbfBlob.arrayBuffer();

            const pbf = new Pbf(pbfArrayBuffer);
            const vectorTile = new VectorTile(pbf);

            const firstFeature = vectorTile.layers.admin.feature(0);

            console.log(firstFeature.toGeoJSON(0, 0, 0));
        };

        const retrieveOsmAsset = async () => {
            const response = await fetch("/assets/maroggia.osm");
            const osmXmlText = await response.text();

            const parser = new DOMParser();
            const xmlDoc = parser.parseFromString(osmXmlText, "text/xml");

            console.log(osmtogeojson(xmlDoc));

            // https://a.tile.openstreetmap.org/10/45/8.png [a,b,c]
            // https://tile.openstreetmap.org/10/45/8.png [a,b,c]
        };

        const loadTiles = async () => {
            const baseImage = new Jimp(600, 400, "#000000");

            const response = await fetch("https://tile.openstreetmap.org/10/45/8.png");
            const tileBlob = await response.blob();
            const tileArrayBuffer = await tileBlob.arrayBuffer();

            const image = await Jimp.read(tileArrayBuffer as Buffer);

            const result = await baseImage.composite(image, 0, 0).getBufferAsync(Jimp.MIME_PNG);

            const blob = new window.Blob([result], { type: Jimp.MIME_PNG });
            const url = window.URL.createObjectURL(blob);
            const a = document.createElement("a");
            document.body.appendChild(a);
            a.setAttribute("style", "display: none");
            a.href = url;
            a.download = "map.png";
            a.click();

            window.URL.revokeObjectURL(url);
            a.remove();
        };

        // retrievePbfAsset();
        // retrieveOsmAsset();

        loadTiles();
    }, []);

    return (
        <div id="app" ref={containerRef}>
            <ReactImgui
                getWasmModule={getWasmModule as GetWasmModule}
                wasmDataPackage={wasmDataPackage as string}
                containerRef={containerRef}
                fontDefs={fontDefs}
                defaultFont={defaultFont}
                styleOverrides={styleOverrides}
            >
                <ImGuiDemo />
            </ReactImgui>
        </div>
    );
}

export default App;

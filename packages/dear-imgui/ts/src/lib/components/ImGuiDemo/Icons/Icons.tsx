import React, { SyntheticEvent, useCallback, useMemo, useRef, useState } from "react";
import { ImGuiCol } from "src/lib/wasm/wasm-app-types";
import faIconMap from "src/lib/fa-icons";
import { ReactImgui } from "../../ReactImgui/components";

export const Icons = () => {
    return (
        <ReactImgui.Node
            style={{
                flexDirection: "row",
                alignItems: "center",
                gap: { column: 5 },
                borderColor: "#000",
                borderThickness: 1,
            }}
        >
            <ReactImgui.UnformattedText text={faIconMap["circle-arrow-right"]} />
            <ReactImgui.Node style={{ width: 4, height: 20, backgroundColor: "#f0cb69" }} />
            <ReactImgui.Node style={{ backgroundColor: "#000000" }}>
                <ReactImgui.UnformattedText
                    text="Inside"
                    style={{
                        colors: { [ImGuiCol.Text]: "#FFFFFF" },
                    }}
                />
            </ReactImgui.Node>
        </ReactImgui.Node>
    );
};

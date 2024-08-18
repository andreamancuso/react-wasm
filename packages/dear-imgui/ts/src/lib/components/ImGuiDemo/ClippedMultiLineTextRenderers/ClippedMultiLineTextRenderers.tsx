import React, { SyntheticEvent, useCallback, useMemo, useRef, useState } from "react";
import { ImGuiCol } from "src/lib/wasm/wasm-app-types";
import faIconMap from "src/lib/fa-icons";
import { ReactImgui } from "../../ReactImgui/components";
import { ClippedMultiLineTextRendererImperativeHandle } from "../../ReactImgui/ClippedMultiLineTextRenderer";

export const ClippedMultiLineTextRenderers = () => {
    const clippedMultiLineTextRendererRef =
        useRef<ClippedMultiLineTextRendererImperativeHandle>(null);

    const handleAppendTextToTextRenderer = useCallback(() => {
        if (clippedMultiLineTextRendererRef.current) {
            clippedMultiLineTextRendererRef.current.appendTextToClippedMultiLineTextRenderer(
                `Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n`.repeat(
                    100,
                ),
            );
        }
    }, [clippedMultiLineTextRendererRef]);

    return (
        <>
            <ReactImgui.Node style={{ flexDirection: "row" }}>
                <ReactImgui.Button onClick={handleAppendTextToTextRenderer} label="Add text" />
            </ReactImgui.Node>
            <ReactImgui.ClippedMultiLineTextRenderer
                ref={clippedMultiLineTextRendererRef}
                style={{ width: 400, height: 400 }}
            />
        </>
    );
};

import React, { SyntheticEvent, useCallback, useMemo, useRef, useState } from "react";
import { ImGuiCol } from "src/lib/wasm/wasm-app-types";
import { ReactImgui } from "../../ReactImgui/components";

export const Images = () => {
    return (
        <ReactImgui.Node
            style={{
                flexDirection: "row",
                gap: { column: 5 },
                alignItems: "center",
                borderColor: "red",
                borderThickness: 2,
                width: "auto",
            }}
        >
            <ReactImgui.Image
                url="https://images.ctfassets.net/hrltx12pl8hq/28ECAQiPJZ78hxatLTa7Ts/2f695d869736ae3b0de3e56ceaca3958/free-nature-images.jpg?fit=fill&w=1200&h=630"
                width={40}
                height={40}
            />

            <ReactImgui.Node
                style={{
                    width: 100,
                    height: 100,
                    backgroundColor: "#000000",
                    borderColor: "lightgreen",
                    borderThickness: 5,
                    rounding: 5,
                    roundCorners: ["topLeft"],
                }}
            >
                <ReactImgui.UnformattedText
                    text="Inside"
                    style={{ colors: { [ImGuiCol.Text]: "#FFFFFF" } }}
                />
                <ReactImgui.Image
                    url="https://images.ctfassets.net/hrltx12pl8hq/28ECAQiPJZ78hxatLTa7Ts/2f695d869736ae3b0de3e56ceaca3958/free-nature-images.jpg?fit=fill&w=1200&h=630"
                    style={{
                        flex: 1,
                    }}
                />
            </ReactImgui.Node>
        </ReactImgui.Node>
    );
};

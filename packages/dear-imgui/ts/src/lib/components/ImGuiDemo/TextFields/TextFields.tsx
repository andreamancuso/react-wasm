import React, { SyntheticEvent, useCallback, useMemo, useRef, useState } from "react";
import { ImGuiCol } from "src/lib/wasm/wasm-app-types";
import faIconMap from "src/lib/fa-icons";
import { ReactImgui } from "../../ReactImgui/components";
import { WidgetReactElement } from "../../ReactImgui/types";

export const TextFields = () => {
    const [text, setText] = useState("Hello, world!");
    const handleInputTextChanged = useCallback(
        (event: SyntheticEvent<WidgetReactElement<"InputText">, { value: string }>) => {
            if (event?.nativeEvent) {
                setText(String(event?.nativeEvent.value));
            }
        },
        [],
    );

    return (
        <>
            <ReactImgui.UnformattedText text="test" />
        </>
    );
};

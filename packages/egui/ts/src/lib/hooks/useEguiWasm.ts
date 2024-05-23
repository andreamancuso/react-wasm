import { useCallback } from "react";
import { Primitive } from "../components/ReactEgui/types";

export const useEguiWasm = (ReactNativePrivateInterface: any) => {
    const onTextChange = useCallback((id: number, value: string) => {
        const rootNodeID = id;
        const topLevelType = "onChange";
        const nativeEventParam = { value };

        ReactNativePrivateInterface.nativeFabricUIManager.dispatchEvent(
            rootNodeID,
            topLevelType,
            nativeEventParam,
        );
    }, []);

    const onComboChange = useCallback((id: number, value: number) => {
        const rootNodeID = id;
        const topLevelType = "onChange";
        const nativeEventParam = { value };

        ReactNativePrivateInterface.nativeFabricUIManager.dispatchEvent(
            rootNodeID,
            topLevelType,
            nativeEventParam,
        );
    }, []);

    const onNumericValueChange = useCallback((id: number, value: number) => {
        const rootNodeID = id;
        const topLevelType = "onChange";
        const nativeEventParam = { value };

        ReactNativePrivateInterface.nativeFabricUIManager.dispatchEvent(
            rootNodeID,
            topLevelType,
            nativeEventParam,
        );
    }, []);

    const onMultiValueChange = useCallback((id: number, values: Primitive[]) => {
        const rootNodeID = id;
        const topLevelType = "onChange";
        const nativeEventParam = { values };

        ReactNativePrivateInterface.nativeFabricUIManager.dispatchEvent(
            rootNodeID,
            topLevelType,
            nativeEventParam,
        );
    }, []);

    const onBooleanValueChange = useCallback((id: number, value: boolean) => {
        const rootNodeID = id;
        const topLevelType = "onChange";
        const nativeEventParam = { value };

        ReactNativePrivateInterface.nativeFabricUIManager.dispatchEvent(
            rootNodeID,
            topLevelType,
            nativeEventParam,
        );
    }, []);

    const onClick = useCallback((id: number) => {
        const rootNodeID = id;
        const topLevelType = "onClick";

        ReactNativePrivateInterface.nativeFabricUIManager.dispatchEvent(rootNodeID, topLevelType, {
            value: "clicked",
        });
    }, []);

    return {
        eventHandlers: {
            on_text_change: onTextChange,
            onComboChange,
            onNumericValueChange,
            onMultiValueChange,
            on_bool_value_change: onBooleanValueChange,
            on_click: onClick,
        },
    };
};

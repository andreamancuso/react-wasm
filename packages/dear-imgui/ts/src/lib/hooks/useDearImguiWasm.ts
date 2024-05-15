import { useCallback } from "react";
import { Primitive } from "../components/ReactImgui/types";

export const useDearImguiWasm = (ReactNativePrivateInterface: any) => {
    const onTextChange = useCallback((id: string, value: string) => {
        const rootNodeID = id;
        const topLevelType = "onChange";
        const nativeEventParam = { value };

        ReactNativePrivateInterface.nativeFabricUIManager.dispatchEvent(
            rootNodeID,
            topLevelType,
            nativeEventParam,
        );
    }, []);

    const onComboChange = useCallback((id: string, value: number) => {
        const rootNodeID = id;
        const topLevelType = "onChange";
        const nativeEventParam = { value };

        ReactNativePrivateInterface.nativeFabricUIManager.dispatchEvent(
            rootNodeID,
            topLevelType,
            nativeEventParam,
        );
    }, []);

    const onNumericValueChange = useCallback((id: string, value: number) => {
        const rootNodeID = id;
        const topLevelType = "onChange";
        const nativeEventParam = { value };

        ReactNativePrivateInterface.nativeFabricUIManager.dispatchEvent(
            rootNodeID,
            topLevelType,
            nativeEventParam,
        );
    }, []);

    const onMultiValueChange = useCallback((id: string, values: Primitive[]) => {
        const rootNodeID = id;
        const topLevelType = "onChange";
        const nativeEventParam = { values };

        ReactNativePrivateInterface.nativeFabricUIManager.dispatchEvent(
            rootNodeID,
            topLevelType,
            nativeEventParam,
        );
    }, []);

    const onBooleanValueChange = useCallback((id: string, value: boolean) => {
        const rootNodeID = id;
        const topLevelType = "onChange";
        const nativeEventParam = { value };

        ReactNativePrivateInterface.nativeFabricUIManager.dispatchEvent(
            rootNodeID,
            topLevelType,
            nativeEventParam,
        );
    }, []);

    const onClick = useCallback((id: string) => {
        const rootNodeID = id;
        const topLevelType = "onClick";

        ReactNativePrivateInterface.nativeFabricUIManager.dispatchEvent(rootNodeID, topLevelType, {
            value: "clicked",
        });
    }, []);

    return {
        eventHandlers: {
            onTextChange,
            onComboChange,
            onNumericValueChange,
            onMultiValueChange,
            onBooleanValueChange,
            onClick,
        },
    };
};

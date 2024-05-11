import * as React from "react";
import { useEffect, useMemo, useState, useRef, useCallback, PropsWithChildren } from "react";
import { v4 as uuidv4 } from "uuid";
// @ts-ignore
import { render } from "../react-native/react-native/libraries/Renderer/implementations/ReactFabric-prod.js";
// @ts-ignore
import * as rnInterface from "../react-native/react-native/libraries/ReactPrivate/ReactNativePrivateInterfaceForFabric";
import debounce from "lodash.debounce";
// @ts-ignore wasm?
import getWasmModule from "src/assets/reactImgui";
import { MainModule, WasmExitStatus } from "src/wasm-app-types";
import { Primitive } from "./components/ReactImgui/types";
import { ReactNativeWrapper } from "src/ReactNativeWrapper";

export type MainComponentProps = PropsWithChildren & {
    containerRef?: React.RefObject<HTMLElement>;
};

export const MainComponent: React.ComponentType<MainComponentProps> = ({
    containerRef,
    children,
}: MainComponentProps) => {
    const canvasRef = useRef<HTMLCanvasElement>(null);

    const isWasmModuleLoading = useRef(false);
    const [wasmModule, setWasmModule] = useState<MainModule | undefined>();
    // const [widgets, setWidgets] = useState<ImguiWidgetsFlat[]>([]);

    const onTextChange = useCallback(
        (id: string, value: string) => {
            const rootNodeID = id;
            const topLevelType = "onChange";
            const nativeEventParam = { value };

            rnInterface.nativeFabricUIManager.dispatchEvent(
                rootNodeID,
                topLevelType,
                nativeEventParam,
            );
        },
        [wasmModule],
    );

    const onComboChange = useCallback((id: string, value: number) => {
        const rootNodeID = id;
        const topLevelType = "onChange";
        const nativeEventParam = { value };

        rnInterface.nativeFabricUIManager.dispatchEvent(rootNodeID, topLevelType, nativeEventParam);
    }, []);

    const onNumericValueChange = useCallback((id: string, value: number) => {
        const rootNodeID = id;
        const topLevelType = "onChange";
        const nativeEventParam = { value };

        rnInterface.nativeFabricUIManager.dispatchEvent(rootNodeID, topLevelType, nativeEventParam);
    }, []);

    const onMultiValueChange = useCallback((id: string, values: Primitive[]) => {
        const rootNodeID = id;
        const topLevelType = "onChange";
        const nativeEventParam = { values };

        rnInterface.nativeFabricUIManager.dispatchEvent(rootNodeID, topLevelType, nativeEventParam);
    }, []);

    const onBooleanValueChange = useCallback((id: string, value: boolean) => {
        const rootNodeID = id;
        const topLevelType = "onChange";
        const nativeEventParam = { value };

        rnInterface.nativeFabricUIManager.dispatchEvent(rootNodeID, topLevelType, nativeEventParam);
    }, []);

    const onClick = useCallback((id: string) => {
        const rootNodeID = id;
        const topLevelType = "onClick";

        rnInterface.nativeFabricUIManager.dispatchEvent(rootNodeID, topLevelType, {
            value: "clicked",
        });
    }, []);

    const canvasId = useMemo(() => `canvas-${uuidv4()}`, []);

    useEffect(() => {
        if (canvasRef.current && !isWasmModuleLoading.current) {
            isWasmModuleLoading.current = true;

            let localModule: MainModule;

            const load = async () => {
                const moduleArg: any = {
                    canvas: canvasRef.current, // ?
                    arguments: [`#${canvasId}`],
                    eventHandlers: {
                        onTextChange,
                        onComboChange,
                        onNumericValueChange,
                        onMultiValueChange,
                        onBooleanValueChange,
                        onClick,
                    },
                };

                localModule = await getWasmModule(moduleArg);

                setWasmModule(localModule);
            };

            load();

            return () => {
                if (localModule) {
                    try {
                        localModule.exit();
                    } catch (error) {
                        if ((error as WasmExitStatus).status !== 0) {
                            // TODO: report error?
                        }
                    }
                }
            };
        } else {
            return () => {};
        }
    }, [
        canvasId,
        canvasRef,
        onTextChange,
        onComboChange,
        onNumericValueChange,
        onBooleanValueChange,
        onMultiValueChange,
        onClick,
    ]);

    useEffect(() => {
        if (wasmModule) {
            if (containerRef?.current) {
                wasmModule.resizeWindow(
                    containerRef.current.clientWidth,
                    containerRef.current.clientHeight - 62,
                );
            }
        }
    }, [wasmModule]);

    useEffect(() => {
        if (wasmModule && containerRef?.current) {
            const resizeObserver = new ResizeObserver(
                debounce(() => {
                    if (containerRef.current) {
                        wasmModule.resizeWindow(
                            containerRef.current.clientWidth,
                            containerRef.current.clientHeight - 62,
                        );
                    }
                }, 20),
            );

            resizeObserver.observe(containerRef.current);

            return () => resizeObserver.disconnect(); // clean up
        } else {
            return () => {};
        }
    }, [wasmModule, containerRef]);

    return (
        <>
            <ReactNativeWrapper wasmModule={wasmModule}>{children}</ReactNativeWrapper>
            <canvas ref={canvasRef} id={canvasId} />
        </>
    );
};

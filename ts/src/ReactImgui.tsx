import * as React from "react";
import { useEffect, useMemo, useState, useRef, useCallback, PropsWithChildren } from "react";
// @ts-ignore
// import { render } from "../react-native/react-native/libraries/Renderer/implementations/ReactNativeRenderer-dev.js";
// @ts-ignore
// import { render } from "../react-native/react-native/libraries/Renderer/implementations/ReactNativeRenderer-prod.js";
// @ts-ignore
import { render } from "../react-native/react-native/libraries/Renderer/implementations/ReactFabric-prod.js";
// @ts-ignore
import * as rnInterface from "../react-native/react-native/libraries/ReactPrivate/ReactNativePrivateInterfaceForFabric";
import { v4 as uuidv4 } from "uuid";
import debounce from "lodash.debounce";
// @ts-ignore wasm?
import getWasmModule from "./assets/reactImgui";
import { WidgetRegistrationServiceContext } from "./contexts/widgetRegistrationServiceContext";

import { MainModule, WasmExitStatus } from "./wasm-app-types";
// import { render } from "./renderer/renderer";
import { WidgetRegistrationService } from "./lib/widgetRegistrationService";
import { resolveWidgets } from "./lib/resolveWidgets";
import { ImguiWidgetsFlat, JSXWidgetNodesFlat, Primitive } from "./components/ReactImgui/types";

export type MainComponentProps = PropsWithChildren & {
    containerRef?: React.RefObject<HTMLElement>;
};

export const MainComponent: React.ComponentType<MainComponentProps> = ({
    containerRef,
    children,
}: MainComponentProps) => {
    // const widgetRegistrationService = useWidgetRegistrationService();

    const widgetRegistrationServiceRef = useRef(new WidgetRegistrationService());
    // const container = useRef<any>();
    const widgetsDefsRef = useRef<any>(null);
    const canvasRef = useRef<HTMLCanvasElement>(null);

    const isWasmModuleLoading = useRef(false);
    const [wasmModule, setWasmModule] = useState<MainModule | undefined>();
    // const [wasmRunner, setWasmRunner] = useState<WasmRunner | undefined>();

    const [widgets, setWidgets] = useState<ImguiWidgetsFlat[]>([]);

    const onTextChange = useCallback(
        (id: string, value: string) => {
            const rootNodeID = id;
            const topLevelType = "onChange";
            const nativeEventParam = { value };

            rnInterface.RCTEventEmitter.propagateEvent(rootNodeID, topLevelType, nativeEventParam);

            // widgetRegistrationServiceRef.current.emitTextInputChangeEvent(id, value);
        },
        [wasmModule],
    );

    const onComboChange = useCallback((id: string, value: number) => {
        const rootNodeID = id;
        const topLevelType = "onChange";
        const nativeEventParam = { value };

        rnInterface.RCTEventEmitter.propagateEvent(rootNodeID, topLevelType, nativeEventParam);
        // setTimeout(() => {
        // widgetRegistrationServiceRef.current.emitComboChangeEvent(id, value);
        // }, 10);
    }, []);

    const onNumericValueChange = useCallback((id: string, value: number) => {
        const rootNodeID = id;
        const topLevelType = "onChange";
        const nativeEventParam = { value };

        rnInterface.RCTEventEmitter.propagateEvent(rootNodeID, topLevelType, nativeEventParam);

        // setTimeout(() => {
        // widgetRegistrationServiceRef.current.emitNumericValueChangeEvent(id, value);
        // }, 10);
    }, []);

    const onMultiValueChange = useCallback((id: string, values: Primitive[]) => {
        const rootNodeID = id;
        const topLevelType = "onChange";
        const nativeEventParam = { values };

        rnInterface.RCTEventEmitter.propagateEvent(rootNodeID, topLevelType, nativeEventParam);

        // setTimeout(() => {
        // widgetRegistrationServiceRef.current.emitMultiValueChangeEvent(id, values);
        // }, 10);
    }, []);

    const onBooleanValueChange = useCallback((id: string, value: boolean) => {
        const rootNodeID = id;
        const topLevelType = "onChange";
        const nativeEventParam = { value };

        // console.log("changed boolean", id, value);

        rnInterface.RCTEventEmitter.propagateEvent(rootNodeID, topLevelType, nativeEventParam);
        // setTimeout(() => {
        // widgetRegistrationServiceRef.current.emitBooleanValueChangeEvent(id, value);
        // }, 10);
    }, []);

    const onClick = useCallback((id: string) => {
        const rootNodeID = id;
        const topLevelType = "onClick";

        // console.log("clicked", id);

        rnInterface.RCTEventEmitter.propagateEvent(rootNodeID, topLevelType, { value: "clicked" });
        // const nativeEventParam = { value };
        // setTimeout(() => {
        // widgetRegistrationServiceRef.current.emitClick(id);
        // }, 10);
    }, []);

    const setWidgetsProxy = useCallback((rawWidgetTree: JSXWidgetNodesFlat[]) => {
        setWidgets(resolveWidgets(rawWidgetTree));
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
                        // localWasmRunner.delete();
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
    }, [wasmModule, widgets]);

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

    // console.log(widgets);

    useEffect(() => {
        if (wasmModule && !widgetsDefsRef.current) {
            // rnInterface.UIManager.init(wasmModule);
            console.log(rnInterface.nativeFabricUIManager, wasmModule);

            rnInterface.nativeFabricUIManager.init(wasmModule);

            render(
                <WidgetRegistrationServiceContext.Provider
                    value={widgetRegistrationServiceRef.current}
                >
                    {children}
                </WidgetRegistrationServiceContext.Provider>,
                0, // containerTag,
                () => {
                    console.log("initialised");
                },
            );

            // AppRegistry.registerComponent("");

            // widgetsDefsRef.current = render(
            //     <WidgetRegistrationServiceContext.Provider
            //         value={widgetRegistrationServiceRef.current}
            //     >
            //         {children}
            //     </WidgetRegistrationServiceContext.Provider>,
            //     { container: setWidgetsProxy },
            // );
        }
    }, [wasmModule, widgetsDefsRef]);

    return (
        <>
            {/* <div id="rootEl" ref={widgetsDefsRef}></div> */}
            {/* <div>
                <pre>{JSON.stringify(widgetsDefsRef.current?.innerHTML)}</pre>
            </div> */}
            <canvas ref={canvasRef} id={canvasId} />
        </>
    );
};

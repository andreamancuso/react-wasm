import * as React from "react";
import { useEffect, useMemo, useState, useRef, useCallback, PropsWithChildren } from "react";
import { v4 as uuidv4 } from "uuid";
import debounce from "lodash.debounce";
// @ts-ignore wasm?
import getWasmModule from "./assets/reactImgui";
import { WidgetRegistrationServiceContext } from "./contexts/widgetRegistrationServiceContext";

import { MainModule, WasmExitStatus, WasmRunner } from "./wasm-app-types";
import { render } from "./renderer/renderer";
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
    const [module, setModule] = useState<MainModule | undefined>();
    const [wasmRunner, setWasmRunner] = useState<WasmRunner | undefined>();

    const [widgets, setWidgets] = useState<ImguiWidgetsFlat[]>([]);

    const onTextChange = useCallback((id: string, value: string) => {
        setTimeout(() => {
            widgetRegistrationServiceRef.current.emitTextInputChangeEvent(id, value);
        }, 10);
    }, []);

    const onComboChange = useCallback((id: string, value: number) => {
        setTimeout(() => {
            widgetRegistrationServiceRef.current.emitComboChangeEvent(id, value);
        }, 10);
    }, []);

    const onNumericValueChange = useCallback((id: string, value: number) => {
        setTimeout(() => {
            widgetRegistrationServiceRef.current.emitNumericValueChangeEvent(id, value);
        }, 10);
    }, []);

    const onMultiValueChange = useCallback((id: string, values: Primitive[]) => {
        setTimeout(() => {
            widgetRegistrationServiceRef.current.emitMultiValueChangeEvent(id, values);
        }, 10);
    }, []);

    const onBooleanValueChange = useCallback((id: string, value: boolean) => {
        setTimeout(() => {
            widgetRegistrationServiceRef.current.emitBooleanValueChangeEvent(id, value);
        }, 10);
    }, []);

    const onClick = useCallback((id: string) => {
        setTimeout(() => {
            widgetRegistrationServiceRef.current.emitClick(id);
        }, 10);
    }, []);

    const setWidgetsProxy = useCallback((rawWidgetTree: JSXWidgetNodesFlat[]) => {
        setWidgets(resolveWidgets(rawWidgetTree));
    }, []);

    const canvasId = useMemo(() => `canvas-${uuidv4()}`, []);

    useEffect(() => {
        if (canvasRef.current && !isWasmModuleLoading.current) {
            isWasmModuleLoading.current = true;

            let localModule: MainModule;
            let localWasmRunner: WasmRunner;

            const load = async () => {
                const moduleArg: any = {
                    canvas: canvasRef.current,
                };

                localModule = await getWasmModule(moduleArg);

                localWasmRunner = new localModule.WasmRunner(
                    onTextChange,
                    onComboChange,
                    onNumericValueChange,
                    onMultiValueChange,
                    onBooleanValueChange,
                    onClick,
                );

                localWasmRunner.run(`#${canvasId}`);

                widgetRegistrationServiceRef.current.setFonts(
                    JSON.parse(localWasmRunner.getAvailableFonts()),
                );

                setModule(localModule);
                setWasmRunner(localWasmRunner);
            };

            load();

            return () => {
                if (localModule) {
                    try {
                        localWasmRunner.delete();
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
        if (wasmRunner) {
            // console.log(JSON.stringify(widgets));

            // console.log("sending widgets");

            wasmRunner.setWidgets(JSON.stringify(widgets));
        }
    }, [wasmRunner, widgets]);

    useEffect(() => {
        if (wasmRunner && containerRef?.current) {
            const resizeObserver = new ResizeObserver(
                debounce(() => {
                    if (containerRef.current) {
                        wasmRunner.resizeWindow(
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
    }, [wasmRunner, containerRef]);

    // console.log(widgets);

    useEffect(() => {
        if (wasmRunner && !widgetsDefsRef.current) {
            widgetsDefsRef.current = render(
                <WidgetRegistrationServiceContext.Provider
                    value={widgetRegistrationServiceRef.current}
                >
                    {children}
                </WidgetRegistrationServiceContext.Provider>,
                { container: setWidgetsProxy },
            );
        }
    }, [wasmRunner, widgetsDefsRef]);

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

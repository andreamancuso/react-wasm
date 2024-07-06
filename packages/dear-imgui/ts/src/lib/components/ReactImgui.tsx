import * as React from "react";
import { useEffect, useMemo, useState, useRef, PropsWithChildren } from "react";
import { v4 as uuidv4 } from "uuid";
import debounce from "lodash.debounce";
// @ts-ignore
import ReactNativePrivateInterface from "../react-native/ReactNativePrivateInterface";
import { GetWasmModule, MainModule, WasmExitStatus } from "../wasm/wasm-app-types";
import { ReactNativeWrapper } from "../components/ReactNativeWrapper";
import { useDearImguiFonts, useDearImguiWasm } from "../hooks";
import { FontDef } from "./ReactImgui/types";
import { ImGuiStyleForPatching } from "../stylesheet/imgui-style";

export type MainComponentProps = PropsWithChildren & {
    containerRef?: React.RefObject<HTMLElement>;
    getWasmModule: GetWasmModule;
    wasm: string;
    wasmDataPackage: string;
    fontDefs?: FontDef[];
    defaultFont?: { name: string; size: number };
    styleOverrides?: ImGuiStyleForPatching;
};

export const MainComponent: React.ComponentType<MainComponentProps> = ({
    containerRef,
    children,
    getWasmModule,
    wasm,
    wasmDataPackage,
    fontDefs,
    defaultFont,
    styleOverrides,
}: MainComponentProps) => {
    const offScreenCanvasRef = useRef<HTMLCanvasElement>(null);
    const canvasRef = useRef<HTMLCanvasElement>(null);

    const isWasmModuleLoading = useRef(false);
    const [wasmModule, setWasmModule] = useState<MainModule | undefined>();

    const canvasId = useMemo(() => `canvas-${uuidv4()}`, []);
    const offScreenCanvasId = useMemo(() => `offscreenCanvas`, []);

    const { eventHandlers } = useDearImguiWasm(ReactNativePrivateInterface);
    const fonts = useDearImguiFonts(fontDefs);

    useEffect(() => {
        if (canvasRef.current) {
            if (!isWasmModuleLoading.current) {
                isWasmModuleLoading.current = true;

                let localModule: MainModule;

                const load = async () => {
                    const args: any[] = [
                        `#${canvasId}`,
                        JSON.stringify({ defs: fonts, defaultFont }),
                    ];

                    if (styleOverrides) {
                        args.push(JSON.stringify(styleOverrides));
                    }

                    const moduleArg: any = {
                        canvas: canvasRef.current,
                        arguments: args,
                        locateFile: (path: string) => {
                            console.log(path);

                            switch (path) {
                                case "reactDearImgui.wasm":
                                    return wasm;
                                case "reactDearImgui.data":
                                    return wasmDataPackage;
                                default:
                                    return path;
                            }
                        },
                        eventHandlers,
                    };

                    try {
                        localModule = await getWasmModule(moduleArg);

                        setWasmModule(localModule);
                    } catch (exception) {
                        console.log("Unable to initialize the WASM correctly", exception);
                    }

                    localModule.initMapStuff();
                };

                load();
            }

            return () => {
                if (wasmModule) {
                    try {
                        wasmModule.exit();
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
    }, [canvasId, canvasRef, fonts, wasmModule, defaultFont, styleOverrides]);

    useEffect(() => {
        if (wasmModule) {
            if (containerRef?.current) {
                try {
                    wasmModule.resizeWindow(
                        containerRef.current.clientWidth,
                        containerRef.current.clientHeight - 62,
                    );
                } catch (exception) {
                    console.log("Unable to set initial window size");
                }
            }
        }
    }, [wasmModule]);

    useEffect(() => {
        if (wasmModule && containerRef?.current) {
            const resizeObserver = new ResizeObserver(
                debounce(() => {
                    if (containerRef.current) {
                        try {
                            wasmModule.resizeWindow(
                                containerRef.current.clientWidth - 1,
                                containerRef.current.clientHeight - 10,
                            );
                        } catch (exception) {
                            console.log("Unable to resize window");
                        }
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
            {wasmModule && (
                <ReactNativeWrapper wasmModule={wasmModule}>{children}</ReactNativeWrapper>
            )}
            {/* <canvas ref={offScreenCanvasRef} id={offScreenCanvasId} /> */}
            <canvas ref={canvasRef} id={canvasId} />
        </>
    );
};

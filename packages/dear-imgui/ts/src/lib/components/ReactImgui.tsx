import * as React from "react";
import { useEffect, useMemo, useState, useRef, useCallback, PropsWithChildren } from "react";
import { v4 as uuidv4 } from "uuid";
import debounce from "lodash.debounce";
// @ts-ignore
import ReactNativePrivateInterface from "../react-native/ReactNativePrivateInterface";
import { MainModule, WasmExitStatus } from "../wasm/wasm-app-types";
import { ReactNativeWrapper } from "../components/ReactNativeWrapper";
import { useDearImguiWasm } from "../hooks";

// getWasmModule.preRun = () => {
//     ENV.MY_FILE_ROOT = "/usr/lib/test";
// };

// console.log(getWasmModule);

export type MainComponentProps = PropsWithChildren & {
    containerRef?: React.RefObject<HTMLElement>;
    getWasmModule: any;
};

export const MainComponent: React.ComponentType<MainComponentProps> = ({
    containerRef,
    children,
    getWasmModule,
}: MainComponentProps) => {
    const canvasRef = useRef<HTMLCanvasElement>(null);

    const isWasmModuleLoading = useRef(false);
    const [wasmModule, setWasmModule] = useState<MainModule | undefined>();

    const canvasId = useMemo(() => `canvas-${uuidv4()}`, []);

    const { eventHandlers } = useDearImguiWasm(ReactNativePrivateInterface);

    useEffect(() => {
        if (canvasRef.current && !isWasmModuleLoading.current) {
            isWasmModuleLoading.current = true;

            let localModule: MainModule;

            const load = async () => {
                const moduleArg: any = {
                    canvas: canvasRef.current, // ?
                    arguments: [`#${canvasId}`],
                    // locateFile: (path: any, scriptDirectory: any) => {
                    //     console.log(path, scriptDirectory);

                    //     return `${scriptDirectory}/lib/wasm/${path}`;
                    // },
                    eventHandlers,
                };

                try {
                    localModule = await getWasmModule(moduleArg);

                    setWasmModule(localModule);
                } catch (exception) {
                    console.log("Unable to initialize the WASM correctly", exception);
                }
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
    }, [canvasId, canvasRef]);

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
                                containerRef.current.clientWidth,
                                containerRef.current.clientHeight - 62,
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
            <canvas ref={canvasRef} id={canvasId} />
        </>
    );
};

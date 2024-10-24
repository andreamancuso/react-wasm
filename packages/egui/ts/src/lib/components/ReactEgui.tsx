import * as React from "react";
import { useEffect, useState, useRef, PropsWithChildren } from "react";
import { v4 as uuidv4 } from "uuid";
// import debounce from "lodash.debounce";
// @ts-ignore
import ReactNativePrivateInterface from "../react-native/ReactNativePrivateInterface";
import { ReactNativeWrapper } from "./ReactNativeWrapper";
import { useEguiWasm } from "../hooks";
import { InitOutput } from "../wasm/eframe_template";

export type MainComponentProps = PropsWithChildren & {
    containerRef?: React.RefObject<HTMLElement>;
    initWasmModule: () => Promise<InitOutput>;
};

export const MainComponent: React.ComponentType<MainComponentProps> = ({
    containerRef,
    children,
    initWasmModule,
}: MainComponentProps) => {
    const canvasRef = useRef<HTMLCanvasElement>(null);

    const isWasmModuleLoading = useRef(false);
    const [wasmModule, setWasmModule] = useState<InitOutput | undefined>();

    // const canvasId = useMemo(() => `canvas-${uuidv4()}`, []);
    const canvasId = "the_canvas_id";

    const { eventHandlers } = useEguiWasm(ReactNativePrivateInterface);

    useEffect(() => {
        if (canvasRef.current && !isWasmModuleLoading.current) {
            isWasmModuleLoading.current = true;

            let localModule: InitOutput;

            const load = async () => {
                try {
                    // @ts-ignore
                    localModule = await initWasmModule.initWasmModule();
                    // @ts-ignore
                    initWasmModule.init_egui(
                        eventHandlers.on_click,
                        eventHandlers.on_text_change,
                        eventHandlers.on_bool_value_change,
                    );

                    // @ts-ignore
                    setWasmModule(initWasmModule);
                } catch (exception) {
                    console.log("Unable to initialize the WASM correctly", exception);
                }
            };

            load();

            // return () => {
            //     if (localModule) {
            //         try {
            //             localModule.exit();
            //         } catch (error) {
            //             if ((error as any).status !== 0) {
            //                 // TODO: report error?
            //             }
            //         }
            //     }
            // };
        } else {
            return () => {};
        }
    }, [canvasId, canvasRef]);

    return (
        <>
            {wasmModule && (
                <ReactNativeWrapper wasmModule={wasmModule}>{children}</ReactNativeWrapper>
            )}
            <canvas ref={canvasRef} id={canvasId} />
        </>
    );
};

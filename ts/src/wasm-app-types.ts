import { Primitive } from "./components/ReactImgui/types";

export type WasmExitStatus = {
    status: number;
};

interface WasmModule {
    ___set_stack_limits(_0: number, _1: number): void;
}

export interface WasmRunner {
    getAvailableFonts(): string;
    exit(): void;
    resizeWindow(_0: number, _1: number): void;
    run(_0: ArrayBuffer | Uint8Array | Uint8ClampedArray | Int8Array | string): void;
    setWidgets(_0: ArrayBuffer | Uint8Array | Uint8ClampedArray | Int8Array | string): void;
    delete(): void;
}

interface EmbindModule {
    WasmRunner: {
        new (
            _0: (id: string, value: string) => void,
            _1: (id: string, value: number) => void,
            _2: (id: string, value: number) => void,
            _3: (id: string, values: Primitive[]) => void,
            _4: (id: string, value: boolean) => void,
            _5: (id: string) => void,
        ): WasmRunner;
    };
    exit(): void;
}
export type MainModule = WasmModule & EmbindModule;

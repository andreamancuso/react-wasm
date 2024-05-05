import { Primitive } from "./components/ReactImgui/types";

export type WasmExitStatus = {
    status: number;
};

// TypeScript bindings for emscripten-generated code.  Automatically generated at compile time.
interface WasmModule {
    ___set_stack_limits(_0: number, _1: number): void;
}

export interface WasmRunner {
    exit(): void;
    resizeWindow(_0: number, _1: number): void;
    run(_0: ArrayBuffer | Uint8Array | Uint8ClampedArray | Int8Array | string): void;
    setWidgets(_0: ArrayBuffer | Uint8Array | Uint8ClampedArray | Int8Array | string): void;
    getAvailableFonts(): string;
    delete(): void;
}

export interface ImGuiWindowFlagsValue<T extends number> {
    value: T;
}
export type ImGuiWindowFlags =
    | ImGuiWindowFlagsValue<0>
    | ImGuiWindowFlagsValue<1>
    | ImGuiWindowFlagsValue<2>
    | ImGuiWindowFlagsValue<4>
    | ImGuiWindowFlagsValue<8>
    | ImGuiWindowFlagsValue<16>
    | ImGuiWindowFlagsValue<32>
    | ImGuiWindowFlagsValue<64>
    | ImGuiWindowFlagsValue<128>
    | ImGuiWindowFlagsValue<256>
    | ImGuiWindowFlagsValue<512>
    | ImGuiWindowFlagsValue<1024>
    | ImGuiWindowFlagsValue<2048>
    | ImGuiWindowFlagsValue<4096>
    | ImGuiWindowFlagsValue<8192>
    | ImGuiWindowFlagsValue<16384>
    | ImGuiWindowFlagsValue<32768>
    | ImGuiWindowFlagsValue<65536>
    | ImGuiWindowFlagsValue<131072>
    | ImGuiWindowFlagsValue<262144>
    | ImGuiWindowFlagsValue<196608>
    | ImGuiWindowFlagsValue<43>
    | ImGuiWindowFlagsValue<197120>
    | ImGuiWindowFlagsValue<1073741824>;

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
    ImGuiWindowFlags: {
        None: ImGuiWindowFlagsValue<0>;
        NoTitleBar: ImGuiWindowFlagsValue<1>;
        NoResize: ImGuiWindowFlagsValue<2>;
        NoMove: ImGuiWindowFlagsValue<4>;
        NoScrollbar: ImGuiWindowFlagsValue<8>;
        NoScrollWithMouse: ImGuiWindowFlagsValue<16>;
        NoCollapse: ImGuiWindowFlagsValue<32>;
        AlwaysAutoResize: ImGuiWindowFlagsValue<64>;
        NoBackground: ImGuiWindowFlagsValue<128>;
        NoSavedSettings: ImGuiWindowFlagsValue<256>;
        NoMouseInputs: ImGuiWindowFlagsValue<512>;
        MenuBar: ImGuiWindowFlagsValue<1024>;
        HorizontalScrollbar: ImGuiWindowFlagsValue<2048>;
        NoFocusOnAppearing: ImGuiWindowFlagsValue<4096>;
        NoBringToFrontOnFocus: ImGuiWindowFlagsValue<8192>;
        AlwaysVerticalScrollbar: ImGuiWindowFlagsValue<16384>;
        AlwaysHorizontalScrollbar: ImGuiWindowFlagsValue<32768>;
        NoNavInputs: ImGuiWindowFlagsValue<65536>;
        NoNavFocus: ImGuiWindowFlagsValue<131072>;
        UnsavedDocument: ImGuiWindowFlagsValue<262144>;
        NoNav: ImGuiWindowFlagsValue<196608>;
        NoDecoration: ImGuiWindowFlagsValue<43>;
        NoInputs: ImGuiWindowFlagsValue<197120>;
        AlwaysUseWindowPadding: ImGuiWindowFlagsValue<1073741824>;
    };
    exit(): void;
    IMGUI_VERSION: any;
}
export type MainModule = WasmModule & EmbindModule;

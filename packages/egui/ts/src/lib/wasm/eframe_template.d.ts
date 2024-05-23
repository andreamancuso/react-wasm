/* tslint:disable */
/* eslint-disable */
/**
* @param {any} on_click
* @param {any} on_text_change
* @param {any} on_bool_value_change
*/
export function init_egui(on_click: any, on_text_change: any, on_bool_value_change: any): void;
/**
* @param {number} parent_id
* @param {number} child_id
*/
export function append_child(parent_id: number, child_id: number): void;
/**
* @param {number} parent_id
* @param {string} raw_children_ids
*/
export function set_children(parent_id: number, raw_children_ids: string): void;
/**
* @returns {string}
*/
export function get_widgets(): string;
/**
* @returns {string}
*/
export function get_hierarchy(): string;
/**
* @param {string} raw_widget_def
*/
export function set_widget(raw_widget_def: string): void;

export type InitInput = RequestInfo | URL | Response | BufferSource | WebAssembly.Module;

export interface InitOutput {
  readonly memory: WebAssembly.Memory;
  readonly append_child: (a: number, b: number) => void;
  readonly set_children: (a: number, b: number, c: number) => void;
  readonly get_widgets: (a: number) => void;
  readonly get_hierarchy: (a: number) => void;
  readonly set_widget: (a: number, b: number) => void;
  readonly init_egui: (a: number, b: number, c: number) => void;
  readonly __wbindgen_malloc: (a: number, b: number) => number;
  readonly __wbindgen_realloc: (a: number, b: number, c: number, d: number) => number;
  readonly __wbindgen_export_2: WebAssembly.Table;
  readonly wasm_bindgen__convert__closures__invoke0_mut__hed3a90fa05699bfe: (a: number, b: number) => void;
  readonly __wbindgen_add_to_stack_pointer: (a: number) => number;
  readonly wasm_bindgen__convert__closures__invoke0_mut__h137fa353aad5688f: (a: number, b: number, c: number) => void;
  readonly wasm_bindgen__convert__closures__invoke1_mut__hac0af3b299a945b8: (a: number, b: number, c: number) => void;
  readonly _dyn_core__ops__function__FnMut__A____Output___R_as_wasm_bindgen__closure__WasmClosure___describe__invoke__hb689abe535ccfe01: (a: number, b: number, c: number) => void;
  readonly __wbindgen_free: (a: number, b: number, c: number) => void;
  readonly __wbindgen_exn_store: (a: number) => void;
}

export type SyncInitInput = BufferSource | WebAssembly.Module;
/**
* Instantiates the given `module`, which can either be bytes or
* a precompiled `WebAssembly.Module`.
*
* @param {SyncInitInput} module
*
* @returns {InitOutput}
*/
export function initSync(module: SyncInitInput): InitOutput;

/**
* If `module_or_path` is {RequestInfo} or {URL}, makes a request and
* for everything else, calls `WebAssembly.instantiate` directly.
*
* @param {InitInput | Promise<InitInput>} module_or_path
*
* @returns {Promise<InitOutput>}
*/
export default function __wbg_init (module_or_path?: InitInput | Promise<InitInput>): Promise<InitOutput>;

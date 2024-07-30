import { v4 as uuidv4 } from "uuid";
import { MainModule } from "./wasm/wasm-app-types";
import { ImGuiStyle } from "./stylesheet/imgui-style";

export class WidgetRegistrationService {
    private wasmModule: MainModule;
    private tables: Set<string>;
    private maps: Set<string>;
    private fabricWidgetsMapping: Map<string, number>;
    private fonts: string[];

    constructor(wasmModule: MainModule) {
        this.wasmModule = wasmModule;
        this.tables = new Set();
        this.maps = new Set();
        this.fabricWidgetsMapping = new Map();
        this.fonts = [];
    }

    setFonts(fonts: string[]) {
        this.fonts = fonts;
    }

    getFonts() {
        return this.fonts;
    }

    getStyle(): ImGuiStyle {
        return JSON.parse(this.wasmModule.getStyle());
    }

    generateId() {
        return uuidv4();
    }

    linkWidgetIds(id: string, fabricId: number) {
        this.fabricWidgetsMapping.set(id, fabricId);
    }

    unlinkWidgetIds(id: string) {
        this.fabricWidgetsMapping.delete(id);
    }

    registerTable(id: string) {
        this.tables.add(id);
    }

    unregisterTable(id: string) {
        this.tables.delete(id);
    }

    registerMap(id: string) {
        this.maps.add(id);
    }

    unregisterMap(id: string) {
        this.maps.delete(id);
    }

    appendDataToTable(id: string, data: any) {
        const fabricWidgetId = this.fabricWidgetsMapping.get(id);
        if (fabricWidgetId !== undefined) {
            try {
                this.wasmModule.appendDataToTable(fabricWidgetId, JSON.stringify(data));
            } catch (error) {
                // todo: propagate this?
                console.error(error);
            }
        }
    }

    renderMap(id: string, centerX: number, centerY: number, zoom: number) {
        const fabricWidgetId = this.fabricWidgetsMapping.get(id);
        if (fabricWidgetId !== undefined) {
            try {
                this.wasmModule.renderMap(fabricWidgetId, centerX, centerY, zoom);
            } catch (error) {
                // todo: propagate this?
                console.error(error);
            }
        }
    }

    appendTextToClippedMultiLineTextRenderer(id: string, text: string) {
        const fabricWidgetId = this.fabricWidgetsMapping.get(id);
        if (fabricWidgetId !== undefined) {
            try {
                this.wasmModule.appendTextToClippedMultiLineTextRenderer(fabricWidgetId, text);
            } catch (error) {
                // todo: propagate this?
                console.error(error);
            }
        }
    }

    setInputTextValue(id: string, value: string) {
        const fabricWidgetId = this.fabricWidgetsMapping.get(id);
        if (fabricWidgetId !== undefined) {
            this.wasmModule.elementInternalOp(
                fabricWidgetId,
                JSON.stringify({ op: "setValue", value }),
            );
        }
    }

    setComboSelectedIndex(id: string, index: number) {
        const fabricWidgetId = this.fabricWidgetsMapping.get(id);
        if (fabricWidgetId !== undefined) {
            this.wasmModule.elementInternalOp(
                fabricWidgetId,
                JSON.stringify({ op: "setSelectedIndex", index }),
            );
        }
    }
}

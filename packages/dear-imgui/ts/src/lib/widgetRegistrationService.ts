import { v4 as uuidv4 } from "uuid";
import { MainModule } from "./wasm/wasm-app-types";
import { ImGuiStyle } from "./stylesheet/imgui-style";

export class WidgetRegistrationService {
    private wasmModule: MainModule;
    private tables: Set<string>;
    private fabricWidgetsMapping: Map<string, number>;
    private fonts: string[];

    constructor(wasmModule: MainModule) {
        this.wasmModule = wasmModule;
        this.tables = new Set();
        this.fabricWidgetsMapping = new Map();
        this.fonts = [];
    }

    getTextLineHeightWithSpacing(): number {
        return this.getTextLineHeightWithSpacing();
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

    appendDataToTable(id: string, data: any) {
        const fabricWidgetId = this.fabricWidgetsMapping.get(id);
        if (fabricWidgetId !== undefined) {
            // todo: we may want to standardize method names
            this.wasmModule.appendDataToTable(fabricWidgetId, JSON.stringify(data));
        }
    }

    appendTextToClippedMultiLineTextRenderer(id: string, text: string) {
        const fabricWidgetId = this.fabricWidgetsMapping.get(id);
        if (fabricWidgetId !== undefined) {
            this.wasmModule.appendTextToClippedMultiLineTextRenderer(fabricWidgetId, text);
        }
    }
}

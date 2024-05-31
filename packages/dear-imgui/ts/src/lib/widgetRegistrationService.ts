import { v4 as uuidv4 } from "uuid";
import { Primitive } from "./components/ReactImgui/types";

export class WidgetRegistrationService {
    private wasmModule: any;
    private tables: Set<string>;
    private fabricWidgetsMapping: Map<string, number>;
    private fonts: string[];

    constructor(wasmModule: any) {
        this.wasmModule = wasmModule;
        this.tables = new Set();
        this.fabricWidgetsMapping = new Map();
        this.fonts = [];
    }

    setFonts(fonts: string[]) {
        this.fonts = fonts;
    }

    getFonts() {
        return this.fonts;
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
        console.log(fabricWidgetId);
        if (fabricWidgetId !== undefined) {
            // todo: we may want to standardize method names
            this.wasmModule.appendDataToTable(fabricWidgetId, JSON.stringify(data));
        }
    }

    appendTextToClippedMultiLineTextRenderer(id: string, text: string) {
        const fabricWidgetId = this.fabricWidgetsMapping.get(id);
        if (fabricWidgetId !== undefined) {
            // console.log("appendTextToClippedMultiLineTextRenderer", id, fabricWidgetId, text);
            this.wasmModule.appendTextToClippedMultiLineTextRenderer(fabricWidgetId, text);
        }
    }
}

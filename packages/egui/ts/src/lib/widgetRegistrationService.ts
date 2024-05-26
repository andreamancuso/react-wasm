import { v4 as uuidv4 } from "uuid";
import { Primitive } from "./components/ReactEgui/types";

export class WidgetRegistrationService {
    private wasmModule: any;
    private tables: Set<string>;
    private fabricWidgetsMapping: Map<string, number>;

    constructor(wasmModule: any) {
        this.wasmModule = wasmModule;
        this.tables = new Set();
        this.fabricWidgetsMapping = new Map();
    }

    generateId() {
        return uuidv4();
    }

    linkWidgetIds(id: string, fabricId: number) {
        console.log("linkWidgetIds", id, fabricId);

        this.fabricWidgetsMapping.set(id, fabricId);
    }

    unlinkWidgetIds(id: string) {
        this.fabricWidgetsMapping.delete(id);
    }

    registerTable(id: string) {
        console.log("registerTable", id);
        this.tables.add(id);
    }

    unregisterTable(id: string) {
        this.tables.delete(id);
    }

    appendDataToTable(id: string, data: any) {
        const fabricWidgetId = this.fabricWidgetsMapping.get(id);
        if (fabricWidgetId !== undefined) {
            console.log("appendDataToTable", id, fabricWidgetId, data);
            this.wasmModule.append_data_to_table(fabricWidgetId, data);
        }
    }
}

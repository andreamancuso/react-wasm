import { Primitive } from "./components/ReactImgui/types";

export class WidgetRegistrationService {
    private textChangeEventMap: Map<string, (value: string) => void>;
    private comboChangeEventMap: Map<string, (value: number) => void>;
    private numericValueChangeEventMap: Map<string, (value: number) => void>;
    private multiValueChangeEventMap: Map<string, (values: Primitive[]) => void>;
    private booleanValueChangeEventMap: Map<string, (value: boolean) => void>;
    private onClickEventMap: Map<string, () => void>;
    private fonts: string[];

    constructor() {
        this.textChangeEventMap = new Map<string, (value: string) => void>();
        this.comboChangeEventMap = new Map<string, (value: number) => void>();
        this.numericValueChangeEventMap = new Map<string, (value: number) => void>();
        this.multiValueChangeEventMap = new Map<string, (values: Primitive[]) => void>();
        this.booleanValueChangeEventMap = new Map<string, (value: boolean) => void>();
        this.onClickEventMap = new Map<string, () => void>();
        this.fonts = [];
    }

    setFonts(fonts: string[]) {
        this.fonts = fonts;
    }

    getFonts() {
        return this.fonts;
    }

    offTextInputChange(id: string) {
        this.textChangeEventMap.delete(id);
    }

    onTextInputChange(id: string, fn: (value: string) => void) {
        this.textChangeEventMap.set(id, fn);
    }

    offComboChange(id: string) {
        this.comboChangeEventMap.delete(id);
    }

    // todo: change value to support anything (using the proper BeginCombo/EndCombo API)
    onComboChange(id: string, fn: (value: number) => void) {
        this.comboChangeEventMap.set(id, fn);
    }

    offNumericValueChange(id: string) {
        this.numericValueChangeEventMap.delete(id);
    }

    onNumericValueChange(id: string, fn: (value: number) => void) {
        this.numericValueChangeEventMap.set(id, fn);
    }

    offMultiValueChange(id: string) {
        this.multiValueChangeEventMap.delete(id);
    }

    onMultiValueChange(id: string, fn: (values: Primitive[]) => void) {
        this.multiValueChangeEventMap.set(id, fn);
    }

    offBooleanValueChange(id: string) {
        this.booleanValueChangeEventMap.delete(id);
    }

    onBooleanValueChange(id: string, fn: (value: boolean) => void) {
        this.booleanValueChangeEventMap.set(id, fn);
    }

    offClick(id: string) {
        this.onClickEventMap.delete(id);
    }

    onClick(id: string, fn: () => void) {
        this.onClickEventMap.set(id, fn);
    }

    emitTextInputChangeEvent(id: string, value: string) {
        const fn = this.textChangeEventMap.get(id);

        if (fn) {
            fn(value);
        }
    }

    emitComboChangeEvent(id: string, value: number) {
        const fn = this.comboChangeEventMap.get(id);

        if (fn) {
            fn(value);
        }
    }

    emitNumericValueChangeEvent(id: string, value: number) {
        const fn = this.numericValueChangeEventMap.get(id);

        if (fn) {
            fn(value);
        }
    }

    emitMultiValueChangeEvent(id: string, values: Primitive[]) {
        const fn = this.multiValueChangeEventMap.get(id);

        if (fn) {
            fn(values);
        }
    }

    emitBooleanValueChangeEvent(id: string, value: boolean) {
        const fn = this.booleanValueChangeEventMap.get(id);

        if (fn) {
            fn(value);
        }
    }

    emitClick(id: string) {
        const fn = this.onClickEventMap.get(id);

        if (fn) {
            fn();
        }
    }
}

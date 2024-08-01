import { FunctionComponent, JSXElementConstructor, ReactElement, SyntheticEvent } from "react";
import { ReactImgui } from "./components";
import { MainComponentProps } from "../ReactImgui";
import { StyleRules } from "src/lib/stylesheet/stylesheet";
import { ImVec2 } from "src/lib/wasm/wasm-app-types";
import { YogaStyle } from "src/lib/stylesheet/yoga-style";

export type ModuleEventHandlers = {
    onTextChange: (id: number, value: string) => void;
    onComboChange: (id: number, value: number) => void;
    onNumericValueChange: (id: number, value: number) => void;
    onMultiValueChange: (id: number, values: Primitive[]) => void;
    onBooleanValueChange: (id: number, value: boolean) => void;
    onClick: (id: number) => void;
    onInit?: () => void;
};

export type WasmDeps = {
    eventHandlers: ModuleEventHandlers;
};

export type Primitive = string | number | boolean;

export type SliderTypes = "default" | "angle";

export type FontDef = {
    name: string;
    sizes: number[];
};

export type StyleProps = {
    style?: StyleRules & YogaStyle;
};

export type TabItemChangeEvent = SyntheticEvent<WidgetReactElement<"TabItem">, { value: boolean }>;

export type InputTextChangeEvent = SyntheticEvent<
    WidgetReactElement<"InputText">,
    { value: string }
>;

export type ComboChangeEvent = SyntheticEvent<WidgetReactElement<"Combo">, { value: number }>;

export type SliderChangeEvent = SyntheticEvent<WidgetReactElement<"Slider">, { value: number }>;

export type MultiSliderChangeEvent = SyntheticEvent<
    WidgetReactElement<"MultiSlider">,
    { value: Primitive[] }
>;

export type CheckboxChangeEvent = SyntheticEvent<
    WidgetReactElement<"Checkbox">,
    { value: boolean }
>;

export type WidgetPropsMap = {
    Unknown: {};
    Child: StyleProps & { width?: number; height?: number };
    DIWindow: StyleProps & { title: string; width: number; height: number };
    Group: StyleProps & {};
    TabBar: StyleProps & {};
    TabItem: StyleProps & {
        label: string;
        onOpenChange?: (event: TabItemChangeEvent) => void;
    };
    ItemTooltip: StyleProps & {};
    TextWrap: StyleProps & { width: number };
    Indent: StyleProps & {};
    UnformattedText: StyleProps & {
        text: string;
    };
    DisabledText: StyleProps & {
        text: string;
    };
    HelpMarker: StyleProps & {
        text: string;
    };
    BulletText: StyleProps & {
        // todo: What about `fmt` ?
        text: string;
    };
    Separator: StyleProps & {};
    SeparatorText: StyleProps & {
        label: string;
    };
    InputText: StyleProps & {
        defaultValue?: string;
        label?: string;
        onChange?: (event: InputTextChangeEvent) => void;
    };
    CollapsingHeader: StyleProps & {
        label?: string;
    };
    TreeNode: StyleProps & {
        label?: string;
    };
    Combo: StyleProps & {
        placeholder?: string;
        options?: string[];
        optionsList?: string;
        initialSelectedIndex?: number;
        onChange?: (event: ComboChangeEvent) => void;
    };
    Slider: StyleProps & {
        sliderType?: SliderTypes;
        label: string;
        defaultValue?: number;
        min?: number;
        max?: number;
        onChange?: (event: SliderChangeEvent) => void;
    };
    MultiSlider: StyleProps & {
        numValues: 2 | 3 | 4;
        label?: string;
        defaultValues?: number[];
        min?: number;
        max?: number;
        decimalDigits?: number;
        onChange?: (event: MultiSliderChangeEvent) => void;
    };
    Checkbox: StyleProps & {
        defaultChecked?: boolean;
        label?: string;
        onChange?: (event: CheckboxChangeEvent) => void;
    };
    Button: StyleProps & {
        onClick?: () => void;
        label?: string;
        size?: ImVec2;
    };
    Table: StyleProps & {
        columns: { heading: string; fieldId?: string }[];
        initialData?: string;
        clipRows?: number;
    };
    ClippedMultiLineTextRenderer: StyleProps & {};
    MapView: StyleProps & {};
    PlotView: StyleProps & {
        xAxisDecimalDigits?: number;
        yAxisDecimalDigits?: number;
        axisAutoFit?: boolean;
    };
};

export type WidgetKeys = keyof WidgetPropsMap;

type ReactImguiType = typeof ReactImgui;

export type WidgetsRequiringId =
    | "InputText"
    | "CollapsingHeader"
    | "TreeNode"
    | "Combo"
    | "Checkbox"
    | "TabBar"
    | "TabItem"
    | "Slider"
    | "MultiSlider"
    | "Button"
    | "Table"
    | "ClippedMultiLineTextRenderer"
    | "MapView"
    | "PlotView";

export type WidgetReactNode =
    | WidgetReactElementsFlat
    | null
    | undefined
    | Iterable<WidgetReactNode>;

export type WidgetReactElement<K extends WidgetKeys> = ReactElement<
    WidgetPropsMap[K],
    JSXElementConstructor<WidgetPropsMap[K]>
>;
type WidgetReactElements = {
    [K in WidgetKeys]: WidgetReactElement<K>;
};
type WidgetReactElementsFlat = WidgetReactElements[keyof WidgetReactElements];

export type ReactImguiTypeKeys = Exclude<
    keyof ReactImguiType,
    keyof React.FunctionComponent<MainComponentProps>
>;

export type ReactElementWidget<
    K extends WidgetKeys,
    P extends WidgetPropsMapFlat = WidgetPropsMapFlat,
> = K extends WidgetsRequiringId
    ? { [L in keyof Omit<P, "children">]: P[L] } & {
          id: string;
          type: K;
          root?: boolean;
          children?: WidgetReactNode;
      } & { onChange?: any; onClick?: any }
    : { [L in keyof Omit<P, "children">]: P[L] } & {
          type: K;
          root?: boolean;
          children?: WidgetReactNode;
      };

type ReactElementWidgets = {
    [K in WidgetKeys]: ReactElementWidget<K, WidgetPropsMap[K]>;
};

export type ReactElementWidgetsFlat = ReactElementWidgets[keyof ReactElementWidgets];

export type YogaNode = {
    root?: boolean;
    style?: YogaStyle;
    children?: WidgetReactNode;
};

export type ImguiWidget<
    K extends WidgetKeys,
    P extends WidgetPropsMapFlat = WidgetPropsMapFlat,
> = K extends WidgetsRequiringId
    ? { [L in keyof Omit<P, "children">]: P[L] } & {
          id: string;
          type: K;
          children?: ImguiWidget<WidgetKeys>[];
      }
    : { [L in keyof Omit<P, "children">]: P[L] } & {
          type: K;
          children?: ImguiWidget<WidgetKeys>[];
      };

type ImguiWidgets = {
    [K in WidgetKeys]: ImguiWidget<K, WidgetPropsMap[K]>;
};

export type ImguiWidgetsFlat = ImguiWidgets[keyof ImguiWidgets];

export type WidgetPropsMapFlat = WidgetPropsMap[keyof WidgetPropsMap];

export type PropsWithChildren<P = unknown> = P & {
    children?: WidgetReactNode;
};

export type WidgetFunctionComponent<P = {}> = FunctionComponent<P> & {
    (props: P): WidgetReactElementsFlat;
};

export type JSXWidgetNode<
    K extends WidgetKeys,
    P extends WidgetPropsMapFlat = WidgetPropsMapFlat,
> = K extends WidgetsRequiringId
    ? {
          type: "widget";
          props: {
              id: string;
              type: K;
          } & P;
          children?: JSXWidgetNodesFlat[];
      }
    : {
          type: "widget";
          props: {
              type: K;
          } & P;
          children?: JSXWidgetNodesFlat[];
      };

type JSXWidgetNodes = {
    [K in WidgetKeys]: JSXWidgetNode<K, WidgetPropsMap[K]>;
};

export type JSXWidgetNodesFlat = JSXWidgetNodes[keyof JSXWidgetNodes];

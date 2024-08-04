import { FunctionComponent, JSXElementConstructor, ReactElement, SyntheticEvent } from "react";
import { ReactImgui } from "./components";
import { MainComponentProps } from "../ReactImgui";
import { StyleRules } from "src/lib/stylesheet/stylesheet";
import { ImVec2 } from "src/lib/wasm/wasm-app-types";
import { YogaStyle } from "src/lib/stylesheet/yoga-style";
import { BaseDrawStyle } from "src/lib/stylesheet/base-draw-style";

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

export type NodeStyle = YogaStyle & BaseDrawStyle;
export type WidgetStyle = StyleRules & YogaStyle & BaseDrawStyle;

export type NodeStyleProps = {
    style?: NodeStyle;
};

export type WidgetStyleProps = {
    style?: WidgetStyle;
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
    Child: WidgetStyleProps;
    DIWindow: WidgetStyleProps & { title: string };
    Group: WidgetStyleProps;
    TabBar: WidgetStyleProps;
    TabItem: WidgetStyleProps & {
        label: string;
        onOpenChange?: (event: TabItemChangeEvent) => void;
    };
    ItemTooltip: WidgetStyleProps;
    TextWrap: WidgetStyleProps & { width: number };
    Indent: WidgetStyleProps;
    UnformattedText: WidgetStyleProps & {
        text: string;
    };
    DisabledText: WidgetStyleProps & {
        text: string;
    };
    HelpMarker: WidgetStyleProps & {
        text: string;
    };
    BulletText: WidgetStyleProps & {
        // todo: What about `fmt` ?
        text: string;
    };
    Separator: WidgetStyleProps;
    SeparatorText: WidgetStyleProps & {
        label: string;
    };
    InputText: WidgetStyleProps & {
        defaultValue?: string;
        label?: string;
        onChange?: (event: InputTextChangeEvent) => void;
    };
    CollapsingHeader: WidgetStyleProps & {
        label?: string;
    };
    TreeNode: WidgetStyleProps & {
        label?: string;
    };
    Combo: WidgetStyleProps & {
        placeholder?: string;
        options?: string[];
        optionsList?: string;
        initialSelectedIndex?: number;
        onChange?: (event: ComboChangeEvent) => void;
    };
    Slider: WidgetStyleProps & {
        sliderType?: SliderTypes;
        label: string;
        defaultValue?: number;
        min?: number;
        max?: number;
        onChange?: (event: SliderChangeEvent) => void;
    };
    MultiSlider: WidgetStyleProps & {
        numValues: 2 | 3 | 4;
        label?: string;
        defaultValues?: number[];
        min?: number;
        max?: number;
        decimalDigits?: number;
        onChange?: (event: MultiSliderChangeEvent) => void;
    };
    Checkbox: WidgetStyleProps & {
        defaultChecked?: boolean;
        label?: string;
        onChange?: (event: CheckboxChangeEvent) => void;
    };
    Button: WidgetStyleProps & {
        onClick?: () => void;
        label?: string;
        size?: ImVec2;
    };
    Table: WidgetStyleProps & {
        columns: { heading: string; fieldId?: string }[];
        initialData?: string;
        clipRows?: number;
    };
    ClippedMultiLineTextRenderer: WidgetStyleProps & {};
    MapView: WidgetStyleProps;
    Image: WidgetStyleProps & { url: string; width?: number; height?: number };
    PlotView: WidgetStyleProps & {
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
    | "Image"
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

export type NodeProps = {
    root?: boolean;
    children?: WidgetReactNode;
} & NodeStyleProps;

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

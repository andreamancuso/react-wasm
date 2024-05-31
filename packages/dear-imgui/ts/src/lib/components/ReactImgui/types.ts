import { FunctionComponent, JSXElementConstructor, ReactElement } from "react";
import { ReactImgui } from "./components";
import { MainComponentProps } from "../ReactImgui";

export type Primitive = string | number | boolean;

export type Vec2 = {
    x: number;
    y: number;
};
export type SliderTypes = "default" | "angle";

export type WidgetPropsMap = {
    Unknown: {};
    Fragment: {};
    Child: { width: number; height: number };
    DIWindow: { title: string };
    Group: {};
    TabBar: {};
    TabItem: { label: string; onOpenChange?: (value: boolean) => void };
    SameLine: {};
    ItemTooltip: {};
    TextWrap: { width: number };
    Indent: {};
    Unindent: {};
    UnformattedText: {
        text: string;
    };
    DisabledText: {
        text: string;
    };
    HelpMarker: {
        text: string;
    };
    BulletText: {
        // todo: What about `fmt` ?
        text: string;
    };
    Separator: {};
    SeparatorText: {
        label: string;
    };
    InputText: {
        defaultValue?: string;
        label?: string;
        onChange?: (value: string) => void;
    };
    CollapsingHeader: {
        label?: string;
    };
    TreeNode: {
        label?: string;
    };
    Combo: {
        label?: string;
        options?: { value: number; label: string }[];
        optionsList?: string;
        defaultValue?: number;
        onChange?: (value: number) => void;
    };
    Slider: {
        sliderType: SliderTypes;
        label: string;
        defaultValue?: number;
        min?: number;
        max?: number;
        onChange?: (value: number) => void;
    };
    MultiSlider: {
        numValues: 2 | 3 | 4;
        label?: string;
        defaultValues?: number[];
        min?: number;
        max?: number;
        decimalDigits?: number;
        onChange?: (values: Primitive[]) => void;
    };
    Checkbox: {
        defaultChecked?: boolean;
        label?: string;
        onChange?: (value: boolean) => void;
    };
    Button: {
        onClick?: () => void;
        label?: string;
        size?: Vec2;
    };
    Table: { columns: { heading: string; fieldId?: string }[]; initialData?: string };
};

type WidgetKeys = keyof WidgetPropsMap;

type ReactImguiType = typeof ReactImgui;

type WidgetsRequiringId =
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
    | "Table";

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
          children?: WidgetReactNode;
      } & { onChange?: any; onClick?: any }
    : { [L in keyof Omit<P, "children">]: P[L] } & {
          type: K;
          children?: WidgetReactNode;
      };

type ReactElementWidgets = {
    [K in WidgetKeys]: ReactElementWidget<K, WidgetPropsMap[K]>;
};

export type ReactElementWidgetsFlat = ReactElementWidgets[keyof ReactElementWidgets];

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

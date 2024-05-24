import { FunctionComponent, JSXElementConstructor, ReactElement } from "react";
import { ReactEgui } from "./components";
import { MainComponentProps } from "../ReactEgui";

export type Primitive = string | number | boolean;

export type WidgetPropsMap = {
    Horizontal: {};
    InputText: { defaultValue?: string; label?: string };
    Checkbox: { label: string; defaultChecked?: boolean; tooltipText?: string };
    Button: {
        label: string;
    };
    CollapsingHeader: { label: string };
};

type WidgetKeys = keyof WidgetPropsMap;

type ReactEguiType = typeof ReactEgui;

type WidgetsWithEvents = "InputText" | "Button" | "CollapsingHeader" | "Checkbox";

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

export type ReactEguiTypeKeys = Exclude<
    keyof ReactEguiType,
    keyof React.FunctionComponent<MainComponentProps>
>;

export type ReactElementWidget<
    K extends WidgetKeys,
    P extends WidgetPropsMapFlat = WidgetPropsMapFlat,
> = K extends WidgetsWithEvents
    ? { [L in keyof Omit<P, "children">]: P[L] } & {
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

export type EguiWidget<
    K extends WidgetKeys,
    P extends WidgetPropsMapFlat = WidgetPropsMapFlat,
> = K extends WidgetsWithEvents
    ? { [L in keyof Omit<P, "children">]: P[L] } & {
          type: K;
          children?: EguiWidget<WidgetKeys>[];
      }
    : { [L in keyof Omit<P, "children">]: P[L] } & {
          type: K;
          children?: EguiWidget<WidgetKeys>[];
      };

type EguiWidgets = {
    [K in WidgetKeys]: EguiWidget<K, WidgetPropsMap[K]>;
};

export type EguiWidgetsFlat = EguiWidgets[keyof EguiWidgets];

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
> = {
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

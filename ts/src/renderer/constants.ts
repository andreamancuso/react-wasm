import {
    ContinuousEventPriority,
    DefaultEventPriority,
    DiscreteEventPriority,
    IdleEventPriority,
} from "react-reconciler/constants";
// import { PoolKey } from "../models/base";
import { styleStringSymbol } from "./diffing";

export type PoolKey = string | number | boolean | null | undefined;

// declare const process;

export const hideClass = "react-unity__renderer__hidden";

export const eventPriorities = {
    discrete: DiscreteEventPriority,
    continuous: ContinuousEventPriority,
    default: DefaultEventPriority,
    idle: IdleEventPriority,
};

export const textTypes = {
    text: true,
    icon: true,
    style: true,
    script: true,
};

export function stringizePoolKey(key: PoolKey) {
    switch (typeof key) {
        case "string":
            return key;
        case "boolean":
            return key ? "default" : "";
        case "number":
            return key.toString();
        case "undefined":
            return null;
        default:
            return "";
    }
}

export function getAllowedProps(props: any, type: keyof typeof textTypes) {
    const { children, tag, pool, ...rest } = props;

    if (textTypes[type] && "children" in props) {
        rest.children =
            !children || typeof children === "boolean"
                ? null
                : Array.isArray(children)
                  ? children.join("")
                  : children + "";
    }

    if (typeof props.style === "string") rest[styleStringSymbol] = props.style;

    return rest;
}

declare const queueMicrotask: (callback: (...args: any[]) => any) => void;

// @ts-ignore @types/node ?
export const isDevelopment = process.env.NODE_ENV === "development";

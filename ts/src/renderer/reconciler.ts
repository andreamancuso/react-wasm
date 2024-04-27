import Reconciler, { OpaqueHandle } from "react-reconciler";
import { DefaultEventPriority } from "react-reconciler/constants";
import { diff } from "deep-object-diff";
import { getAllowedProps, hideClass, stringizePoolKey, textTypes } from "./constants";
import { diffProperties } from "./diffing";
import { ImguiWidgetsFlat, JSXWidgetNodesFlat } from "../components/ReactImgui/types";

type Type = any;
type Props = any;
export type Container = (rawWidgetTree: JSXWidgetNodesFlat[]) => void;
type Instance = any;
type TextInstance = any;
type SuspenceInstance = any;
type HydratableInstance = any;
type PublicInstance = any;
type HostContext = any;
type UpdatePayload = any;
type ChildSet = JSXWidgetNodesFlat[];
type TimeoutHandle = any;
type NoTimeout = any;

type HostConfig = Reconciler.HostConfig<
    Type,
    Props,
    Container,
    Instance,
    TextInstance,
    SuspenceInstance,
    HydratableInstance,
    PublicInstance,
    HostContext,
    UpdatePayload,
    ChildSet,
    TimeoutHandle,
    NoTimeout
>;

const hostContext = {};
const childContext = {};

const hostConfig: HostConfig = {
    // -------------------
    //     Scheduling
    // -------------------

    // now: Date.now,
    getCurrentEventPriority: () => DefaultEventPriority,

    noTimeout: -1 as const,
    scheduleTimeout: (callback: any, delay: number) => setTimeout(callback as any, delay),
    scheduleMicrotask:
        typeof queueMicrotask === "function"
            ? queueMicrotask
            : (callback: any) =>
                  Promise.resolve(null)
                      .then(callback)
                      .catch((error) =>
                          setTimeout(() => {
                              throw error;
                          }, 0),
                      ),
    cancelTimeout: (handle: any) => clearTimeout(handle),

    beforeActiveInstanceBlur() {},

    afterActiveInstanceBlur() {},

    getInstanceFromNode(_node: any) {
        return undefined;
    },
    getInstanceFromScope(_scopeInstance: any) {
        return undefined;
    },

    prepareScopeUpdate(_scopeInstance: any, _instance: any) {},

    getRootHostContext: () => hostContext,
    getChildHostContext: () => childContext,
    getPublicInstance: (instance: any) => {
        // console.log("instance", instance);
        return instance;
    },

    supportsMutation: false,
    supportsHydration: false,
    supportsPersistence: true,
    supportsMicrotasks: true, // ?
    // supportsTestSelectors: false,

    isPrimaryRenderer: true,
    warnsIfNotActing: true, //

    prepareForCommit: () => null,
    resetAfterCommit: () => {},

    prepareUpdate(
        _instance: Instance,
        _type: Type,
        rawOldProps: Props,
        rawNewProps: Props,
        _rootContainer: Container,
        _hostContext: HostContext,
    ): UpdatePayload | null {
        const { children: _rawOldChildren, ...oldProps } = rawOldProps;
        const { children: _rawNewChildren, ...newProps } = rawNewProps;

        const diffs = diff(oldProps, newProps);

        return Object.keys(diffs).length > 0;
    },

    // clearContainer: (container) => {
    //     console.log("clear container");
    // },
    // shouldDeprioritizeSubtree: () => false,

    createInstance(type, props, rootContainerInstance) {
        // console.log("createInstance", type, props);

        const { children, ...otherProps } = props;

        return { type, props: otherProps, children: [] };
    },

    appendInitialChild(parent, child) {
        // console.log("appendInitialChild", parent, child);

        // parent.children = parent.children ?? {};

        parent.children.push(child);
    },
    finalizeInitialChildren: () => false,
    // commitMount: () => {},
    shouldSetTextContent(type) {
        return false;
    },

    cloneInstance(
        instance: Instance,
        _updatePayload: UpdatePayload,
        _type: Type,
        _oldProps: Props,
        newProps: Props,
        _internalInstanceHandle: OpaqueHandle,
        _keepChildren: boolean,
        _recyclableInstance: null | Instance,
    ): Instance {
        const { children, ...props } = newProps;

        return { ...instance, props, children: [] };
    },
    createContainerChildSet(container: Container): ChildSet {
        // console.log("createContainerChildSet");

        return [];
    },
    appendChildToContainerChildSet(childSet: ChildSet, child: Instance | TextInstance): void {
        // console.log("appendChildToContainerChildSet");

        const { children, ...props } = child.props;
        const normalizedChild = { ...child, props };

        childSet.push(normalizedChild);
    },
    finalizeContainerChildren(container: Container, newChildren: ChildSet): void {
        // console.log("finalizeContainerChildren");

        container(newChildren);
    },
    replaceContainerChildren(container: Container, newChildren: ChildSet): void {
        // console.log("replaceContainerChildren");

        container(newChildren);
    },
    cloneHiddenInstance(
        instance: Instance,
        type: Type,
        props: Props,
        internalInstanceHandle: OpaqueHandle,
    ): Instance {
        console.log("cloneHiddenInstance", instance, type, props, internalInstanceHandle);

        return instance;
    },
    cloneHiddenTextInstance(
        instance: Instance,
        text: Type,
        internalInstanceHandle: OpaqueHandle,
    ): TextInstance {
        console.log("cloneHiddenTextInstance", instance, text, internalInstanceHandle);

        return instance;
    },

    createTextInstance() {
        throw new Error("text instance are not supported");
    },

    resetTextContent: () => {},
    preparePortalMount: () => {},
    detachDeletedInstance: () => {},

    // Required for Suspense

    hideInstance: (instance) => {
        instance.ClassList.Add(hideClass);
    },
    hideTextInstance: (instance) => {
        instance.ClassList.Add(hideClass);
    },
    unhideInstance: (instance) => {
        instance.ClassList.Remove(hideClass);
    },
    unhideTextInstance: (instance) => {
        instance.ClassList.Remove(hideClass);
    },
};

export const syncReconciler = Reconciler(hostConfig);

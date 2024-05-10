// import PQueue from "p-queue";

// const queue = new PQueue({ concurrency: 10 });

// queue.on("empty", () => {
//     console.log("queue is empty");
// });

// queue.on("completed", () => {
//     console.log("job completed");
// });
const test = new Map();

class NativeFabricUIManager {
    wasmModule;
    dispatchEventFn;
    cloningNode;

    constructor() {}

    init(wasmModule) {
        this.wasmModule = wasmModule;
    }
    dispatchEvent = (rootNodeID, topLevelType, nativeEventParam) => {
        this.dispatchEventFn(test.get(rootNodeID), topLevelType, nativeEventParam);
    };
    createNode = (generatedId, uiViewClassName, requiresClone, payload, fiberNode) => {
        // console.log("createNode", generatedId, uiViewClassName, requiresClone, payload, fiberNode);

        // todo: yikes
        if (this.cloningNode) {
            this.cloningNode = null;
        }

        const { children, type, id, ...props } = payload;
        const widget = { ...props, id: generatedId, type };

        // queue.add(() => {
        // console.time("createView");

        this.wasmModule.setWidget(JSON.stringify(widget));
        // console.timeEnd("createView");
        // });

        test.set(generatedId, fiberNode);

        return widget;
    };
    cloneNodeWithNewProps = (node, newProps) => {
        // console.log("cloneNodeWithNewProps", node, newProps);

        const newWidget = { ...node, ...newProps };

        // queue.add(() => {
        this.wasmModule.patchWidget(node.id, JSON.stringify(newWidget));
        // });

        // console.log("cloneNodeWithNewProps", JSON.stringify(newWidget));

        return newWidget;
    };
    // This is a rather problematic method
    // I should drop all children then re-append them but naturally this causes out of memory bounds issues
    // Also, I still don't understand at which point I am supposed to delete widgets/nodes
    // todo: have a look at react-native's own implementation of this...
    cloneNodeWithNewChildren = (node) => {
        // todo: yikes
        if (this.cloningNode) {
            this.wasmModule.setChildren(
                this.cloningNode.id,
                JSON.stringify(this.cloningNode.childrenIds),
            );
        }

        this.cloningNode = { id: node.id, childrenIds: [] };

        // todo: does this make sense?
        return node;
    };
    createChildSet(...args) {
        // console.log("createChildSet", args);

        return [];
    }
    appendChildToSet(set, child) {
        // console.log("appendChildToSet", set, child);

        set.push(child);
    }
    appendChild = (parent, child) => {
        // console.log("appendChild", parent, child);

        // todo: yikes
        if (this.cloningNode) {
            this.cloningNode.childrenIds.push(child.id);
        } else {
            // queue.add(() => {
            this.wasmModule.appendChild(parent.id, child.id);
            // });
        }
    };
    completeRoot = (container, newChildSet) => {
        // console.log("completeRoot", container, newChildSet);

        // todo: yikes
        if (this.cloningNode) {
            const cloningNodeId = this.cloningNode.id;
            const payload = JSON.stringify(this.cloningNode.childrenIds);
            // queue.add(() => {
            this.wasmModule.setChildren(cloningNodeId, payload);
            // });
            this.cloningNode = null;
        }

        const payload = JSON.stringify(newChildSet.map(({ id }) => id));

        // queue.add(() => {
        this.wasmModule.setChildren(container, payload);
        // });
    };
    registerEventHandler = (dispatchEventFn) => {
        this.dispatchEventFn = dispatchEventFn;
    };
}

const uiManager = new NativeFabricUIManager();

// flowlint unsafe-getters-setters:off
module.exports = {
    createPublicInstance(current, renderLanes, workInProgress) {
        // console.log("createPublicInstance", current, renderLanes, workInProgress);

        return {};
    },
    get BatchedBridge() {
        return {};
    },
    get ExceptionsManager() {
        return {};
    },
    get Platform() {
        return {};
    },
    get ReactNativeViewConfigRegistry() {
        return {
            customBubblingEventTypes: {},
            customDirectEventTypes: {
                onChange: { registrationName: "onChange" },
                onClick: { registrationName: "onClick" },
            },
            get(elementType, ...unknownArgs) {
                switch (elementType) {
                    case "widget":
                        return {
                            validAttributes: {
                                type: true,
                                id: true,
                                label: true,
                                text: true,
                                defaultValues: true,
                                defaultValue: true,
                                min: true,
                                max: true,
                                width: true,
                                optionsList: true,
                                sliderType: true,
                                numValues: true,
                                decimalDigits: true,
                                defaultChecked: true,
                                size: true,
                                onChange: true,
                                onClick: true,
                            },
                        };
                }

                return {
                    validAttributes: [],
                };
            },
        };
    },
    get TextInputState() {
        return {};
    },
    get nativeFabricUIManager() {
        return uiManager;
    },
    // TODO: Remove when React has migrated to `createAttributePayload` and `diffAttributePayloads`
    get deepDiffer() {
        return require("../../../deepDiffer");
    },
    get deepFreezeAndThrowOnMutationInDev() {
        // Applicable only in DEV mode
        // return (...args) => console.log("deepFreezeAndThrowOnMutationInDev", args);
        return (...args) => {};
    },
    get flattenStyle() {
        return {};
    },
    get ReactFiberErrorDialog() {
        return {
            showErrorDialog(...args) {
                console.log("ReactFiberErrorDialog.showErrorDialog", args);
            },
        };
    },
    get legacySendAccessibilityEvent() {
        return {};
    },
    get RawEventEmitter() {
        return {
            emit(...args) {
                console.log(args);
            },
        };
    },
    get CustomEvent() {
        return {};
    },
    get createAttributePayload() {
        return {};
    },
    get diffAttributePayloads() {
        return {};
    },
    get createPublicTextInstance() {
        return {};
    },
    get getNativeTagFromPublicInstance() {
        return {};
    },
    get getNodeFromPublicInstance() {
        return {};
    },
    get getInternalInstanceHandleFromPublicInstance() {
        return {};
    },
};

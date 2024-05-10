import PQueue from "p-queue";

const queue = new PQueue({ concurrency: 10 });

// queue.on("empty", () => {
//     console.log("queue is empty");
// });

// queue.on("completed", () => {
//     console.log("job completed");
// });

class NativeFabricUIManager {
    wasmModule;

    constructor() {}

    init(wasmModule) {
        this.wasmModule = wasmModule;
    }

    removeRootView(containerTag) {
        console.log("removeRootView", containerTag);
    }
    createNode = (generatedId, uiViewClassName, requiresClone, payload, fiberNode) => {
        console.log("createNode", generatedId, uiViewClassName, requiresClone, payload, fiberNode);

        const { children, type, id, ...props } = payload;
        const widget = { ...props, id: generatedId, type };

        // queue.add(() => {
        // console.time("createView");
        this.wasmModule.setWidget(JSON.stringify(widget));
        // console.timeEnd("createView");
        // });

        return widget;
    };
    createChildSet(...args) {
        console.log("createChildSet", args);

        return [];
    }
    appendChildToSet(set, child) {
        console.log("appendChildToSet", set, child);

        set.push(child);
    }
    appendChild = (parent, child) => {
        console.log("appendChild", parent, child);
        this.wasmModule.appendChild(parent.id, child.id);
    };
    completeRoot = (container, newChildSet) => {
        console.log("completeRoot", container, newChildSet);

        this.wasmModule.setChildren(container, JSON.stringify(newChildSet.map(({ id }) => id)));
    };
    createView(generatedId, uiViewClassName, rootContainerInstance, payload) {
        // console.log(
        //     "UIManager.createView",
        //     generatedId,
        //     uiViewClassName,
        //     rootContainerInstance,
        //     payload,
        // );

        queue.add(() => {
            // console.time("createView");
            const { children, type, id, ...props } = payload;
            const widget = { ...props, id: generatedId, type };

            this.wasmModule.setWidget(JSON.stringify(widget));
            // console.timeEnd("createView");
        });
    }
    updateView(generatedId, className, payload) {
        // console.log("updateView", generatedId, className, payload);

        queue.add(() => {
            // console.time("updateView");
            const { children, type, id, ...props } = payload;
            this.wasmModule.patchWidget(generatedId, JSON.stringify(props));
            // console.timeEnd("updateView");
        });
    }
    setChildren(id, childrenIds) {
        // console.log("UIManager.setChildren", id, childrenIds);

        queue.add(() => {
            // console.time("setChildren");
            this.wasmModule.setChildren(id, JSON.stringify(childrenIds));
            // console.timeEnd("setChildren");
        });
    }
    manageChildren(
        id,
        moveFromIndices,
        moveToIndices,
        addChildReactTags,
        addAtIndices,
        removeAtIndices,
    ) {
        // console.log(
        //     "manageChildren",
        //     id,
        //     moveFromIndices,
        //     moveToIndices,
        //     addChildReactTags,
        //     addAtIndices,
        //     removeAtIndices,
        // );

        // todo: this is seriously slow
        queue.add(() => {
            // console.time("manageChildren");
            const children = JSON.parse(this.wasmModule.getChildren(id));

            if (addChildReactTags.length > 0 && addAtIndices.length > 0) {
                addChildReactTags.forEach((addChildReactTag, index) => {
                    const addAtIndex = addAtIndices[index];

                    children.splice(addAtIndex, 0, ...addChildReactTags);
                });
            }

            if (removeAtIndices.length > 0) {
                children.splice(removeAtIndices[0], 1);
            }

            this.wasmModule.setChildren(id, JSON.stringify(children));
            // console.timeEnd("manageChildren");
        });
    }
}

class RCTEventEmitter {
    re;
    rt;

    register = ({ receiveEvent, receiveTouches }) => {
        this.re = receiveEvent;
        this.rt = receiveTouches;
    };

    propagateEvent = (rootNodeID, topLevelType, nativeEventParam) => {
        this.re(rootNodeID, topLevelType, nativeEventParam);
    };
}

const uiManager = new NativeFabricUIManager();
const rctEventEmitter = new RCTEventEmitter();

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
    get RCTEventEmitter() {
        return {};
        // return rctEventEmitter;
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
        return {};
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

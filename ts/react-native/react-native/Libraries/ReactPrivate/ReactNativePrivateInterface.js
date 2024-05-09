class UIManager {
    wasmModule;
    views;
    hierarchy;

    constructor() {
        this.views = new Map();
        this.hierarchy = new Map();
    }

    init(wasmModule) {
        this.wasmModule = wasmModule;
    }

    removeRootView(containerTag) {
        console.log("removeRootView", containerTag);
    }
    createView(generatedId, uiViewClassName, rootContainerInstance, payload) {
        // console.log(
        //     "UIManager.createView",
        //     generatedId,
        //     uiViewClassName,
        //     rootContainerInstance,
        //     payload,
        // );

        const { children, type, id, ...props } = payload;

        const widget = { ...props, id: generatedId, type };

        this.views.set(generatedId, widget);
        this.hierarchy.set(generatedId, []);

        // console.log(JSON.stringify(widget));

        this.wasmModule.setWidget(JSON.stringify(widget));
        this.wasmModule.setChildren(generatedId, []);
    }
    updateView(generatedId, className, payload) {
        // console.log("updateView", generatedId, className, payload);

        const { children, type, id, ...props } = payload;

        this.wasmModule.patchWidget(generatedId, JSON.stringify(props));
    }
    setChildren(id, childrenIds) {
        // console.log("UIManager.setChildren", id, childrenIds);
        this.hierarchy.set(id, childrenIds);
        this.wasmModule.setChildren(id, childrenIds);
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

        const children = this.wasmModule.getChildren(id);

        // console.log(children);

        if (addChildReactTags.length > 0 && addAtIndices.length > 0) {
            addChildReactTags.forEach((addChildReactTag, index) => {
                const addAtIndice = addAtIndices[index];

                children.splice(addAtIndice, 0, ...addChildReactTags);
            });
        }

        if (removeAtIndices.length > 0) {
            children.splice(removeAtIndices[0], 1);
        }

        this.wasmModule.setChildren(id, children);
    }
}

class RCTEventEmitter {
    re;
    rt;

    // init(receiveEvent, receiveTouches) {
    //     re = receiveEvent;
    //     rt = receiveTouches;
    // }

    register({ receiveEvent, receiveTouches }) {
        this.re = receiveEvent;
        this.rt = receiveTouches;
    }

    propagateEvent(rootNodeID, topLevelType, nativeEventParam) {
        this.re(rootNodeID, topLevelType, nativeEventParam);
    }
}

const uiManager = new UIManager();
const rctEventEmitter = new RCTEventEmitter();

// flowlint unsafe-getters-setters:off
module.exports = {
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
        return rctEventEmitter;
    },
    get ReactNativeViewConfigRegistry() {
        return {
            customBubblingEventTypes: {
                //     onChange: { registrationName: "onChange", phasedRegistrationNames: [] },
            },
            customDirectEventTypes: {
                onChange: { registrationName: "onChange" },
                onClick: { registrationName: "onClick" },
            },
            get(elementType, ...unknownArgs) {
                // console.log("ReactNativeViewConfigRegistry.get", elementType, unknownArgs);

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

                // console.log(elementType);

                return {
                    validAttributes: [],
                };
            },
        };
    },
    get TextInputState() {
        return {};
    },
    get UIManager() {
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
    get createPublicInstance() {
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

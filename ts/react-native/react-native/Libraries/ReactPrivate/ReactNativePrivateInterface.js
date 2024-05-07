/**
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 *
 * @format
 * @flow strict-local
 */

const views = new Map();
const hierarchy = new Map();

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
        return {
            register(...args) {
                // console.log("RCTEventEmitter.register", args);
            },
        };
    },
    get ReactNativeViewConfigRegistry() {
        return {
            get(elementType, ...unknownArgs) {
                // console.log("ReactNativeViewConfigRegistry.get", elementType, unknownArgs);

                switch (elementType) {
                    case "widget":
                        return {
                            validAttributes: {
                                type: true,
                                id: true,
                                label: true,
                                children: true,
                                defaultValues: true,
                                min: true,
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
    get UIManager() {
        return {
            removeRootView(containerTag) {
                console.log("removeRootView", containerTag);
            },
            createView(id, uiViewClassName, rootContainerInstance, payload) {
                // console.log(
                //     "UIManager.createView",
                //     id,
                //     uiViewClassName,
                //     rootContainerInstance,
                //     payload,
                // );

                const { children, type, ...props } = payload;

                views.set(id, { id, type, props });
                hierarchy.set(id, []);
            },
            updateView(id, className, payload) {
                console.log("updateView", id, className, payload);
            },
            setChildren(id, childIds) {
                // console.log("UIManager.setChildren", targetReactTag, childReactTags);
                hierarchy.set(id, childIds);
            },
            manageChildren(
                id,
                moveFromIndices,
                moveToIndices,
                addChildReactTags,
                addAtIndices,
                removeAtIndices,
            ) {
                console.log(
                    "manageChildren",
                    id,
                    moveFromIndices,
                    moveToIndices,
                    addChildReactTags,
                    addAtIndices,
                    removeAtIndices,
                );
            },
        };
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

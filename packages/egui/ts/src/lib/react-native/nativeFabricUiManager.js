export default class {
    wasmModule;
    widgetRegistrationService;
    dispatchEventFn;
    cloningNode;
    fiberNodesMap;

    constructor() {
        this.fiberNodesMap = new Map();
    }

    init(wasmModule, widgetRegistrationService) {
        this.wasmModule = wasmModule;
        this.widgetRegistrationService = widgetRegistrationService;
    }
    dispatchEvent = (rootNodeID, topLevelType, nativeEventParam) => {
        // todo: understand the implications of using setTimeout() here, which is currently required
        // as events are sent while the WIDGETS Mutex is locked -> this is the only way to prevent a deadlock at the moment
        setTimeout(() => {
            this.dispatchEventFn(
                this.fiberNodesMap.get(rootNodeID),
                topLevelType,
                nativeEventParam,
            );
        }, 0);
    };
    createNode = (generatedId, uiViewClassName, requiresClone, payload, fiberNode) => {
        console.log("createNode", generatedId, uiViewClassName, requiresClone, payload, fiberNode);

        // todo: yikes
        if (this.cloningNode) {
            this.cloningNode = null;
        }

        const { children, type, id, ...props } = payload;
        const widget = { ...props, id: generatedId, type };

        console.log(JSON.stringify(widget));

        this.wasmModule.set_widget(JSON.stringify(widget));

        this.fiberNodesMap.set(generatedId, fiberNode);

        // todo: type is in some array of types
        if (type === "Table") {
            this.widgetRegistrationService.linkWidgetIds(id, generatedId);
        }

        return widget;
    };
    cloneNodeWithNewProps = (node, newProps) => {
        const newWidget = { ...node, ...newProps };
        // this.wasmModule.patchWidget(node.id, JSON.stringify(newWidget));

        return newWidget;
    };
    // This is a rather problematic method
    // I should drop all children then re-append them but naturally this causes out of memory bounds issues
    // Also, I still don't understand at which point I am supposed to delete widgets/nodes
    // todo: have a look at react-native's own implementation of this...
    cloneNodeWithNewChildren = (node) => {
        // todo: yikes
        if (this.cloningNode) {
            this.wasmModule.set_children(
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
            this.wasmModule.append_child(parent.id, child.id);
        }
    };
    completeRoot = (container, newChildSet) => {
        // console.log("completeRoot", container, newChildSet);

        // todo: yikes
        if (this.cloningNode) {
            const cloningNodeId = this.cloningNode.id;
            const payload = JSON.stringify(this.cloningNode.childrenIds);
            this.wasmModule.set_children(cloningNodeId, payload);
            this.cloningNode = null;
        }

        const payload = JSON.stringify(newChildSet.map(({ id }) => id));

        this.wasmModule.set_children(container, payload);
    };
    registerEventHandler = (dispatchEventFn) => {
        this.dispatchEventFn = dispatchEventFn;
    };
}

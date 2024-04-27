import * as React from "react";
import { Container, syncReconciler } from "./reconciler";
import { ConcurrentRoot } from "react-reconciler/constants";

// const containerMap = new Map<
//     NativeContainerInstance | number,
//     { hostRoot: any; asyncJobCallback: () => void }
// >();

interface RenderOptions {
    /* Unity element to render React on. It is the element `ReactUnity` component is attached to by default. */
    // hostContainer?: NativeContainerInstance;

    /* Skips rendering some useful wrappers like `ErrorBoundary` and `GlobalsProvider`. */
    disableHelpers?: boolean;

    container: Container;
}

// let renderCount = 0;

export function render(element: React.ReactNode, { container }: RenderOptions) {
    const hostRoot = syncReconciler.createContainer(
        container,
        ConcurrentRoot,
        null,
        false,
        null,
        "",
        (error) => console.error(error),
        null,
    );

    const rc = syncReconciler;
    rc.updateContainer(element as any, hostRoot, null);

    rc.injectIntoDevTools({
        bundleType: 0, // 0 is PROD, 1 is DEV
        version: "22",
        rendererPackageName: "react-imgui/renderer",
        rendererConfig: { isAsync: false },
        findFiberByHostInstance: (_instance: any) => null,
    });

    return rc;
}

// export const batchedUpdates = asyncReconciler.batchedUpdates;
// export const flushSync = asyncReconciler.flushSync;

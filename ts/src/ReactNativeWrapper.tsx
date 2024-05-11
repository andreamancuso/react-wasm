import * as React from "react";
import { useEffect, useRef, PropsWithChildren } from "react";
// @ts-ignore
import { render } from "../react-native/react-native/libraries/Renderer/implementations/ReactFabric-prod.js";
// @ts-ignore
import * as rnInterface from "../react-native/react-native/libraries/ReactPrivate/ReactNativePrivateInterfaceForFabric";
import { WidgetRegistrationServiceContext } from "./contexts/widgetRegistrationServiceContext";
import { WidgetRegistrationService } from "./lib/widgetRegistrationService";
import { MainModule } from "./wasm-app-types";

export type ReactNativeWrapperProps = PropsWithChildren & {
    wasmModule?: MainModule;
};

export const ReactNativeWrapper: React.ComponentType<ReactNativeWrapperProps> = ({
    wasmModule,
    children,
}: ReactNativeWrapperProps) => {
    const widgetRegistrationServiceRef = useRef(new WidgetRegistrationService());
    const initialisedRef = useRef(false);

    useEffect(() => {
        if (wasmModule && !initialisedRef.current) {
            initialisedRef.current = true;

            rnInterface.nativeFabricUIManager.init(wasmModule);

            render(
                <WidgetRegistrationServiceContext.Provider
                    value={widgetRegistrationServiceRef.current}
                >
                    {children}
                </WidgetRegistrationServiceContext.Provider>,
                0, // containerTag,
                () => {
                    // console.log("initialised");
                },
                1,
            );
        }
    }, [wasmModule, initialisedRef]);

    return null;
};

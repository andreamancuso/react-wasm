import * as React from "react";
import { useEffect, useRef, PropsWithChildren } from "react";
// @ts-ignore
import ReactFabricProdInitialiser from "src/lib/react-native/ReactFabric-prod";
// @ts-ignore
import ReactNativePrivateInterface from "src/lib/react-native/ReactNativePrivateInterface";
import { WidgetRegistrationServiceContext } from "src/lib/contexts/widgetRegistrationServiceContext";
import { WidgetRegistrationService } from "src/lib/widgetRegistrationService";
import { MainModule } from "src/lib/wasm/wasm-app-types";

const ReactFabricProd = ReactFabricProdInitialiser(ReactNativePrivateInterface);

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

            // todo: inject via Context
            ReactNativePrivateInterface.nativeFabricUIManager.init(wasmModule);

            ReactFabricProd.render(
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

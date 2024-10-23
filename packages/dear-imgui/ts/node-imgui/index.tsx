import * as React from "react";

import { ReactFabricProd } from "./ReactNativeWrapper";
import { WidgetRegistrationServiceContext } from "../src/lib/contexts/widgetRegistrationServiceContext";
import { WidgetRegistrationService } from "../src/lib/widgetRegistrationService";
import ReactNativePrivateInterface from "../src/lib/react-native/ReactNativePrivateInterface";
import { App } from "./App";

const nodeImgui = require("../../cpp/node/build/Release/node-imgui.node");

nodeImgui.init();

const widgetRegistrationService = new WidgetRegistrationService(nodeImgui);

ReactNativePrivateInterface.nativeFabricUIManager.init(nodeImgui, widgetRegistrationService);

setTimeout(() => {
    ReactFabricProd.render(
        <WidgetRegistrationServiceContext.Provider value={widgetRegistrationService}>
            <App />
        </WidgetRegistrationServiceContext.Provider>,
        0, // containerTag,
        () => {
            // console.log("initialised");
        },
        1,
    );

    nodeImgui.showDebugWindow();
}, 500);

let flag = true;
(function keepProcessRunning() {
    setTimeout(() => flag && keepProcessRunning(), 1000);
})();

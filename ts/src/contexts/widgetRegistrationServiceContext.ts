import { createContext } from "react";
import { WidgetRegistrationService } from "../lib/widgetRegistrationService";

export const WidgetRegistrationServiceContext = createContext<WidgetRegistrationService | null>(
    null,
);

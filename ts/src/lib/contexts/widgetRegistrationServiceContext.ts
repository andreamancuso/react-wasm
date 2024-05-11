import { createContext } from "react";
import { WidgetRegistrationService } from "src/lib/widgetRegistrationService";

export const WidgetRegistrationServiceContext = createContext<WidgetRegistrationService | null>(
    null,
);

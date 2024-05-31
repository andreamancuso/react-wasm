import { forwardRef, useEffect, useImperativeHandle, useRef } from "react";
import { useWidgetRegistrationService } from "../../hooks";
import { WidgetPropsMap } from "./types";

export type ClippedMultiLineTextRendererImperativeHandle = {
    appendTextToClippedMultiLineTextRenderer: (data: string) => void;
};

export const ClippedMultiLineTextRenderer = forwardRef<
    ClippedMultiLineTextRendererImperativeHandle,
    WidgetPropsMap["ClippedMultiLineTextRenderer"]
>(({}: WidgetPropsMap["ClippedMultiLineTextRenderer"], ref) => {
    const widgetRegistratonService = useWidgetRegistrationService();
    const idRef = useRef(widgetRegistratonService.generateId());

    useEffect(() => {
        widgetRegistratonService.registerTable(idRef.current);
    }, [widgetRegistratonService]);

    useImperativeHandle(
        ref,
        () => {
            return {
                appendTextToClippedMultiLineTextRenderer(data: string) {
                    widgetRegistratonService.appendTextToClippedMultiLineTextRenderer(
                        idRef.current,
                        data,
                    );
                },
            };
        },
        [],
    );

    return <widget type="ClippedMultiLineTextRenderer" id={idRef.current} />;
});

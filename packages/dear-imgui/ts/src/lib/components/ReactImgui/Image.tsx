import { forwardRef, useEffect, useImperativeHandle, useRef } from "react";
import { WidgetPropsMap } from "./types";
import { useWidgetRegistrationService } from "../../hooks";

export type MapImperativeHandle = {
    reload: () => void;
};

export const Image = forwardRef<MapImperativeHandle, WidgetPropsMap["Image"]>(
    ({ url, width, height, style }: WidgetPropsMap["Image"], ref) => {
        const widgetRegistratonService = useWidgetRegistrationService();
        const idRef = useRef(widgetRegistratonService.generateId());

        useEffect(() => {
            widgetRegistratonService.registerMap(idRef.current);
        }, [widgetRegistratonService]);

        useImperativeHandle(
            ref,
            () => {
                return {
                    reload() {
                        widgetRegistratonService.reloadImage(idRef.current);
                    },
                };
            },
            [],
        );

        return (
            <widget
                type="Image"
                id={idRef.current}
                style={style}
                url={url}
                width={width}
                height={height}
            />
        );
    },
);

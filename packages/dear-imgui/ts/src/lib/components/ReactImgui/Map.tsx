import { forwardRef, useEffect, useImperativeHandle, useRef } from "react";
import { WidgetPropsMap } from "./types";
import { useWidgetRegistrationService } from "../../hooks";

export type MapImperativeHandle = {
    render: (centerX: number, centerY: number, zoom: number) => void;
};

export const MapComponent = forwardRef<MapImperativeHandle, WidgetPropsMap["Map"]>(
    ({ style }: WidgetPropsMap["Map"], ref) => {
        const widgetRegistratonService = useWidgetRegistrationService();
        const idRef = useRef(widgetRegistratonService.generateId());

        useEffect(() => {
            widgetRegistratonService.registerMap(idRef.current);
        }, [widgetRegistratonService]);

        useImperativeHandle(
            ref,
            () => {
                return {
                    render(centerX: number, centerY: number, zoom: number) {
                        widgetRegistratonService.renderMap(idRef.current, centerX, centerY, zoom);
                    },
                };
            },
            [],
        );

        return <widget type="Map" id={idRef.current} style={style} />;
    },
);

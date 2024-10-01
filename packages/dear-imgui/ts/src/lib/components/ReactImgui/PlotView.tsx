import { forwardRef, useEffect, useImperativeHandle, useRef } from "react";
import { useWidgetRegistrationService } from "../../hooks";
import { WidgetPropsMap } from "./types";

export type PlotViewImperativeHandle = {
    appendData: (x: number, y: number) => void;
    setAxesDecimalDigits: (x: number, y: number) => void;
    setAxesAutoFit: (enabled: boolean) => void;
    resetData: () => void;
};

export const PlotView = forwardRef<PlotViewImperativeHandle, WidgetPropsMap["PlotView"]>(
    (
        {
            xAxisDecimalDigits,
            yAxisDecimalDigits,
            xAxisScale,
            yAxisScale,
            axisAutoFit,
            style,
        }: WidgetPropsMap["PlotView"],
        ref,
    ) => {
        const widgetRegistratonService = useWidgetRegistrationService();
        const idRef = useRef(widgetRegistratonService.generateId());

        useEffect(() => {
            widgetRegistratonService.registerTable(idRef.current);
        }, [widgetRegistratonService]);

        useImperativeHandle(
            ref,
            () => {
                return {
                    appendData: (x: number, y: number) => {
                        widgetRegistratonService.appendDataToPlotView(idRef.current, x, y);
                    },
                    setAxesDecimalDigits: (x: number, y: number) => {
                        widgetRegistratonService.setPlotViewAxesDecimalDigits(idRef.current, x, y);
                    },
                    setAxesAutoFit: (enabled: boolean) => {
                        widgetRegistratonService.setPlotViewAutoAxisFitEnabled(
                            idRef.current,
                            enabled,
                        );
                    },
                    resetData: () => {
                        widgetRegistratonService.resetPlotViewData(idRef.current);
                    },
                };
            },
            [],
        );

        return (
            <plot-view
                id={idRef.current}
                xAxisDecimalDigits={xAxisDecimalDigits}
                yAxisDecimalDigits={yAxisDecimalDigits}
                xAxisScale={xAxisScale}
                yAxisScale={yAxisScale}
                axisAutoFit={axisAutoFit}
                style={style}
            />
        );
    },
);

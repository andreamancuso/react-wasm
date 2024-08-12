import { forwardRef, useEffect, useImperativeHandle, useRef } from "react";
import { useWidgetRegistrationService } from "../../hooks";
import { WidgetPropsMap } from "./types";

export type TableImperativeHandle = {
    appendDataToTable: (data: any[]) => void;
};

export const Table = forwardRef<TableImperativeHandle, WidgetPropsMap["Table"]>(
    ({ columns, clipRows, initialData, style }: WidgetPropsMap["Table"], ref) => {
        const widgetRegistratonService = useWidgetRegistrationService();
        const idRef = useRef(widgetRegistratonService.generateId());

        useEffect(() => {
            widgetRegistratonService.registerTable(idRef.current);
        }, [widgetRegistratonService]);

        useImperativeHandle(
            ref,
            () => {
                return {
                    appendDataToTable(data: any[]) {
                        widgetRegistratonService.appendDataToTable(idRef.current, data);
                    },
                };
            },
            [],
        );

        return <di-table id={idRef.current} columns={columns} clipRows={clipRows} style={style} />;
    },
);

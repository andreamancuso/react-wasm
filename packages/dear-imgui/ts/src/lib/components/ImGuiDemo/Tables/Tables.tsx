import React, { useCallback, useMemo, useRef, useState } from "react";
import { ReactImgui } from "src/lib/components/ReactImgui/components";
import { TableImperativeHandle } from "../../ReactImgui/Table";

const sampleData = [
    { id: "1", name: "Name" },
    { id: "1", name: "Name" },
    { id: "1", name: "Name" },
    { id: "1", name: "Name" },
    { id: "1", name: "Name" },
];

export const Tables = () => {
    const intervalRef = useRef<NodeJS.Timeout>();
    const counterRef = useRef<number>(1);
    const tableRef1 = useRef<TableImperativeHandle>(null);
    const tableRef2 = useRef<TableImperativeHandle>(null);
    const tableRef3 = useRef<TableImperativeHandle>(null);
    const tableRef4 = useRef<TableImperativeHandle>(null);

    const tableColumns = useMemo(
        () => [
            {
                heading: "ID",
                fieldId: "id",
            },
            {
                heading: "Name",
                fieldId: "name",
            },
        ],
        [],
    );

    const handleAppendDataToTableClick = useCallback(() => {
        if (tableRef1.current && tableRef2.current && tableRef3.current && tableRef4.current) {
            intervalRef.current = setInterval(() => {
                if (
                    tableRef1.current &&
                    tableRef2.current &&
                    tableRef3.current &&
                    tableRef4.current
                ) {
                    const data = sampleData.map((sampleDataRow, index) => ({
                        ...sampleDataRow,
                        id: `${counterRef.current + index}`,
                    }));
                    counterRef.current += sampleData.length;

                    // console.log(counterRef.current);

                    tableRef1.current.appendDataToTable(data);
                    tableRef2.current.appendDataToTable(data);
                    tableRef3.current.appendDataToTable(data);
                    tableRef4.current.appendDataToTable(data);
                }
            }, 1);
        }
    }, [tableRef1, tableRef2, tableRef3]);

    const handleStopAppendingDataToTableClick = useCallback(() => {
        if (intervalRef.current) {
            clearInterval(intervalRef.current);
        }
    }, []);

    return (
        <ReactImgui.Fragment>
            <ReactImgui.SameLine>
                <ReactImgui.Child width={250} height={-40}>
                    <ReactImgui.Table ref={tableRef1} columns={tableColumns} clipRows={10} />
                </ReactImgui.Child>
                <ReactImgui.Child width={250} height={-40}>
                    <ReactImgui.Table ref={tableRef2} columns={tableColumns} clipRows={10} />
                </ReactImgui.Child>
                <ReactImgui.Child width={250} height={-40}>
                    <ReactImgui.Table ref={tableRef3} columns={tableColumns} clipRows={10} />
                </ReactImgui.Child>
                <ReactImgui.Child width={250} height={-40}>
                    <ReactImgui.Table ref={tableRef4} columns={tableColumns} clipRows={10} />
                </ReactImgui.Child>
            </ReactImgui.SameLine>
            <ReactImgui.SameLine>
                <ReactImgui.Button
                    onClick={handleAppendDataToTableClick}
                    label="Add data to table"
                />
                <ReactImgui.Button
                    onClick={handleStopAppendingDataToTableClick}
                    label="Stop adding data"
                />
            </ReactImgui.SameLine>
        </ReactImgui.Fragment>
    );
};

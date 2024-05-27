import { useCallback, useMemo, useRef } from "react";
import { ReactEgui } from "../ReactEgui/components";
import { TableImperativeHandle } from "../ReactEgui/Table";

export const EguiDemo = () => {
    const tableRef = useRef<TableImperativeHandle>(null);

    const handleClick = useCallback(() => {
        console.log("click!");
    }, []);

    const handleInputTextChange = useCallback((event: any) => {
        console.log("changed text:", event.nativeEvent.value);
    }, []);

    const handleCheckboxCheckedChange = useCallback((event: any) => {
        console.log("changed checked state:", event.nativeEvent.value);
    }, []);

    const handleCollapsingHeaderChange = useCallback((event: any) => {
        console.log("changed collapsing header state:", event.nativeEvent.value);
    }, []);

    const handleRadioButtonGroupChange = useCallback((event: any) => {
        console.log("changed radio button group value:", event.nativeEvent.value);
    }, []);

    const radioButtonGroupOptions = useMemo(
        () => [
            { label: "A", value: "a", tooltipText: "A means a" },
            { label: "B", value: "b", tooltipText: "B means b" },
            { label: "C", value: "c", tooltipText: "C means c" },
        ],
        [],
    );

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
        if (tableRef.current) {
            tableRef.current.appendDataToTable([{ id: "1", name: "Andy" }]);
        }
    }, [tableRef]);

    return (
        <ReactEgui.CollapsingHeader
            onChange={handleCollapsingHeaderChange}
            label="Click to reveal contents"
        >
            <ReactEgui.Horizontal>
                <ReactEgui.InputText
                    onChange={handleInputTextChange}
                    defaultValue="Hello, world!"
                />
                <ReactEgui.Checkbox
                    onChange={handleCheckboxCheckedChange}
                    defaultChecked
                    label="Hello, world?"
                    tooltipText="Yes, hello, world!!!"
                />

                <ReactEgui.Button onClick={handleClick} label="Hello, world!" />
            </ReactEgui.Horizontal>
            <ReactEgui.Horizontal>
                <ReactEgui.RadioButton label="A" />
                <ReactEgui.RadioButton label="B" />
                <ReactEgui.RadioButton label="C" />
            </ReactEgui.Horizontal>
            <ReactEgui.Horizontal>
                <ReactEgui.RadioButtonGroup
                    defaultValue="a"
                    onChange={handleRadioButtonGroupChange}
                    options={radioButtonGroupOptions}
                />
            </ReactEgui.Horizontal>
            <ReactEgui.Table ref={tableRef} columns={tableColumns} />
            <ReactEgui.Button onClick={handleAppendDataToTableClick} label="Add data to table" />
        </ReactEgui.CollapsingHeader>
    );
};

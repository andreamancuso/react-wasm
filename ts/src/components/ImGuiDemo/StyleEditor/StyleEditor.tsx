import { ReactImgui } from "src/lib/components/ReactImgui/components";
import { BorderControl } from "src/components/ImGuiDemo/StyleEditor/BorderControl/BorderControl";
import { FontSelector } from "src/components/ImGuiDemo/StyleEditor/FontSelector/FontSelector";
import { FrameRounding } from "src/components/ImGuiDemo/StyleEditor/Sizes/Rounding/FrameRounding/FrameRounding";
import { HelpMarker } from "src/components/ImGuiDemo/HelpMarker/HelpMarker";
import { StyleSelector } from "src/components/ImGuiDemo/StyleEditor/StyleSelector/StyleSelector";
import { Main } from "src/components/ImGuiDemo/StyleEditor/Sizes/Main";
import { Borders } from "src/components/ImGuiDemo/StyleEditor/Sizes/Borders";
import { Rounding } from "./Sizes/Rounding";
import { Tables } from "./Sizes/Tables";
import { Widgets } from "./Sizes/Widgets";
import { Tooltips } from "./Sizes/Tooltips";
import { Misc } from "./Sizes/Misc";
import { useCallback, useState } from "react";

export const StyleEditor = () => {
    const [items, setItems] = useState<any[]>([]);
    const addItem = useCallback(() => setItems((value) => [...value, "hello"]), []);
    const addManyItems = useCallback(
        () => setItems((items) => items.concat(Array.from({ length: 200 }, (_, i) => i + 1))),
        [],
    );
    const resetItems = useCallback(() => setItems([]), []);

    const removeItem = useCallback(
        (index: number) => setItems((value) => value.filter((_item, i) => i !== index)),
        [],
    );

    return (
        <ReactImgui.Fragment>
            <StyleSelector />
            <FontSelector />
            <FrameRounding />
            <BorderControl />
            <ReactImgui.SameLine>
                <ReactImgui.Button label="Save Ref" onClick={addItem} />
                <ReactImgui.Button label="Add Many" onClick={addManyItems} />
                <ReactImgui.Button label="Revert Ref" onClick={resetItems} />
                <HelpMarker text='Save/Revert in local non-persistent storage. Default Colors definition are not affected. Use "Export" below to save them somewhere.' />
                <ReactImgui.UnformattedText text={`${items.length}`} />
            </ReactImgui.SameLine>
            {items.map((item, index) => (
                <ReactImgui.SameLine key={index}>
                    <ReactImgui.UnformattedText text={`${item}`} />
                    <ReactImgui.Button onClick={() => removeItem(index)} label="X" />
                </ReactImgui.SameLine>
            ))}
            <ReactImgui.Separator />
            <ReactImgui.TabBar>
                <ReactImgui.TabItem label="Sizes">
                    <ReactImgui.SeparatorText label="Main" />
                    <Main />
                    <ReactImgui.SeparatorText label="Borders" />
                    <Borders />
                    <ReactImgui.SeparatorText label="Rounding" />
                    <Rounding />
                    <ReactImgui.SeparatorText label="Tables" />
                    <Tables />
                    <ReactImgui.SeparatorText label="Widgets" />
                    <Widgets />
                    <ReactImgui.SeparatorText label="Tooltips" />
                    <Tooltips />
                    <ReactImgui.SeparatorText label="Misc" />
                    <Misc />
                </ReactImgui.TabItem>
                <ReactImgui.TabItem label="Colors">
                    <ReactImgui.SeparatorText label="Main" />
                </ReactImgui.TabItem>
                <ReactImgui.TabItem label="Fonts">
                    <ReactImgui.SeparatorText label="Main" />
                </ReactImgui.TabItem>
                <ReactImgui.TabItem label="Rendering">
                    <ReactImgui.SeparatorText label="Main" />
                </ReactImgui.TabItem>
            </ReactImgui.TabBar>
        </ReactImgui.Fragment>
    );
};

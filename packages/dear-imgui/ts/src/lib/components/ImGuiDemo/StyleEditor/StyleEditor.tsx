import { ReactImgui } from "src/lib/components/ReactImgui/components";
import { BorderControl } from "./BorderControl/BorderControl";
import { FontSelector } from "./FontSelector/FontSelector";
import { FrameRounding } from "./Sizes/Rounding/FrameRounding/FrameRounding";
import { HelpMarker } from "../HelpMarker/HelpMarker";
import { StyleSelector } from "./StyleSelector/StyleSelector";
import { Main } from "./Sizes/Main";
import { Borders } from "./Sizes/Borders";
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
        <ReactImgui.Node>
            <StyleSelector />
            <FontSelector />
            <FrameRounding />
            <BorderControl />
            <ReactImgui.Node style={{ flexDirection: "row" }}>
                <ReactImgui.Button label="Save Ref" onClick={addItem} />
                <ReactImgui.Button label="Add Many" onClick={addManyItems} />
                <ReactImgui.Button label="Revert Ref" onClick={resetItems} />
                <HelpMarker text='Save/Revert in local non-persistent storage. Default Colors definition are not affected. Use "Export" below to save them somewhere.' />
                <ReactImgui.UnformattedText text={`${items.length}`} />
            </ReactImgui.Node>
            {items.map((item, index) => (
                <ReactImgui.Node style={{ flexDirection: "row" }} key={index}>
                    <ReactImgui.UnformattedText text={`${item}`} />
                    <ReactImgui.Button onClick={() => removeItem(index)} label="X" />
                </ReactImgui.Node>
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
                    <ReactImgui.UnformattedText text="Coming soon!" />
                </ReactImgui.TabItem>
                <ReactImgui.TabItem label="Fonts">
                    <ReactImgui.UnformattedText text="Coming soon!" />
                </ReactImgui.TabItem>
                <ReactImgui.TabItem label="Rendering">
                    <ReactImgui.UnformattedText text="Coming soon!" />
                </ReactImgui.TabItem>
            </ReactImgui.TabBar>
        </ReactImgui.Node>
    );
};

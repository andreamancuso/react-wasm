import { ReactImgui } from "src/components/ReactImgui/components";
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
import { useCallback } from "react";

export const StyleEditor = () => {
    const handleClick = useCallback(() => console.log("clicked"), []);

    return (
        <ReactImgui.Fragment>
            <StyleSelector />
            <FontSelector />
            <FrameRounding />
            <BorderControl />
            <ReactImgui.SameLine>
                <ReactImgui.Button label="Save Ref" onClick={handleClick} />
                <ReactImgui.Button label="Revert Ref" onClick={handleClick} />
                <HelpMarker text='Save/Revert in local non-persistent storage. Default Colors definition are not affected. Use "Export" below to save them somewhere.' />
            </ReactImgui.SameLine>
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

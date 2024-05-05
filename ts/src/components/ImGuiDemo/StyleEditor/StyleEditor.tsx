import { ReactImgui } from "src/components/ReactImgui/components";
import { BorderControl } from "src/components/ImGuiDemo/StyleEditor/BorderControl/BorderControl";
import { FontSelector } from "src/components/ImGuiDemo/StyleEditor/FontSelector/FontSelector";
import { FrameRounding } from "src/components/ImGuiDemo/StyleEditor/Sizes/Rounding/FrameRounding/FrameRounding";
import { HelpMarker } from "src/components/ImGuiDemo/HelpMarker/HelpMarker";
import { StyleSelector } from "src/components/ImGuiDemo/StyleEditor/StyleSelector/StyleSelector";
import { GrabMinSize } from "src/components/ImGuiDemo/StyleEditor/Sizes/Main/GrabMinSize/GrabMinSize";
import { ItemInnerSpacing } from "src/components/ImGuiDemo/StyleEditor/Sizes/Main/ItemInnerSpacing/ItemInnerSpacing";
import { FramePadding } from "src/components/ImGuiDemo/StyleEditor/Sizes/Main/FramePadding/FramePadding";
import { IndentSpacing } from "src/components/ImGuiDemo/StyleEditor/Sizes/Main/IndentSpacing/IndentSpacing";
import { ItemSpacing } from "src/components/ImGuiDemo/StyleEditor/Sizes/Main/ItemSpacing/ItemSpacing";
import { ScrollbarSize } from "src/components/ImGuiDemo/StyleEditor/Sizes/Main/ScrollbarSize/ScrollbarSize";
import { TouchExtraPadding } from "src/components/ImGuiDemo/StyleEditor/Sizes/Main/TouchExtraPadding/TouchExtraPadding";
import { WindowPadding } from "src/components/ImGuiDemo/StyleEditor/Sizes/Main/WindowPadding/WindowPadding";
import { WindowBorderSize } from "src/components/ImGuiDemo/StyleEditor/Sizes/Borders/WindowBorderSize/WindowBorderSize";
import { ChildBorderSize } from "src/components/ImGuiDemo/StyleEditor/Sizes/Borders/ChildBorderSize/ChildBorderSize";
import { PopupBorderSize } from "src/components/ImGuiDemo/StyleEditor/Sizes/Borders/PopupBorderSize/PopupBorderSize";
import { FrameBorderSize } from "src/components/ImGuiDemo/StyleEditor/Sizes/Borders/FrameBorderSize/FrameBorderSize";
import { TabBorderSize } from "src/components/ImGuiDemo/StyleEditor/Sizes/Borders/TabBorderSize/TabBorderSize";
import { TabBarBorderSize } from "src/components/ImGuiDemo/StyleEditor/Sizes/Borders/TabBarBorderSize/TabBarBorderSize";
import { WindowRounding } from "src/components/ImGuiDemo/StyleEditor/Sizes/Rounding/WindowRounding/WindowRounding";
import { ChildRounding } from "src/components/ImGuiDemo/StyleEditor/Sizes/Rounding/ChildRounding/ChildRounding";
import { PopupRounding } from "src/components/ImGuiDemo/StyleEditor/Sizes/Rounding/PopupRounding/PopupRounding";
import { ScrollbarRounding } from "src/components/ImGuiDemo/StyleEditor/Sizes/Rounding/ScrollbarRounding/ScrollbarRounding";
import { GrabRounding } from "src/components/ImGuiDemo/StyleEditor/Sizes/Rounding/GrabRounding/GrabRounding";
import { TabRounding } from "src/components/ImGuiDemo/StyleEditor/Sizes/Rounding/TabRounding/TabRounding";
import { CellPadding } from "src/components/ImGuiDemo/StyleEditor/Sizes/Tables/CellPadding/CellPadding";
import { TableAngledHeadersAngle } from "src/components/ImGuiDemo/StyleEditor/Sizes/Tables/TableAngledHeadersAngle/TableAngledHeadersAngle";
import { WindowTitleAlign } from "src/components/ImGuiDemo/StyleEditor/Sizes/Widgets/WindowTitleAlign/WindowTitleAlign";
import { WindowMenuButtonPosition } from "src/components/ImGuiDemo/StyleEditor/Sizes/Widgets/WindowMenuButtonPosition/WindowMenuButtonPosition";
import { ColorButtonPosition } from "src/components/ImGuiDemo/StyleEditor/Sizes/Widgets/ColorButtonPosition/ColorButtonPosition";
import { ButtonTextAlign } from "src/components/ImGuiDemo/StyleEditor/Sizes/Widgets/ButtonTextAlign/ButtonTextAlign";
import { SelectableTextAlign } from "src/components/ImGuiDemo/StyleEditor/Sizes/Widgets/SelectableTextAlign/SelectableTextAlign";
import { SeparatorTextBorderSize } from "src/components/ImGuiDemo/StyleEditor/Sizes/Widgets/SeparatorTextBorderSize/SeparatorTextBorderSize";
import { SeparatorTextAlign } from "src/components/ImGuiDemo/StyleEditor/Sizes/Widgets/SeparatorTextAlign/SeparatorTextAlign";
import { SeparatorTextPadding } from "src/components/ImGuiDemo/StyleEditor/Sizes/Widgets/SeparatorTextPadding/SeparatorTextPadding";
import { LogSliderDeadzone } from "src/components/ImGuiDemo/StyleEditor/Sizes/Widgets/LogSliderDeadzone/LogSliderDeadzone";

export const StyleEditor = () => {
    return (
        <>
            <StyleSelector />
            <FontSelector />
            <FrameRounding />
            <BorderControl />
            <ReactImgui.SameLine>
                <ReactImgui.Button label="Save Ref" />
                <ReactImgui.Button label="Revert Ref" />
                <HelpMarker text='Save/Revert in local non-persistent storage. Default Colors definition are not affected. Use "Export" below to save them somewhere.' />
            </ReactImgui.SameLine>
            <ReactImgui.Separator />
            <ReactImgui.TabBar>
                <ReactImgui.TabItem label="Sizes">
                    <ReactImgui.SeparatorText label="Main" />
                    <WindowPadding />
                    <FramePadding />
                    <ItemSpacing />
                    <ItemInnerSpacing />
                    <TouchExtraPadding />
                    <IndentSpacing />
                    <ScrollbarSize />
                    <GrabMinSize />
                    <ReactImgui.SeparatorText label="Borders" />
                    <WindowBorderSize />
                    <ChildBorderSize />
                    <PopupBorderSize />
                    <FrameBorderSize />
                    <TabBorderSize />
                    <TabBarBorderSize />
                    <ReactImgui.SeparatorText label="Rounding" />
                    <WindowRounding />
                    <ChildRounding />
                    <FrameRounding />
                    <PopupRounding />
                    <ScrollbarRounding />
                    <GrabRounding />
                    <TabRounding />
                    <ReactImgui.SeparatorText label="Tables" />
                    <CellPadding />
                    <TableAngledHeadersAngle />
                    <ReactImgui.SeparatorText label="Widgets" />
                    <WindowTitleAlign />
                    <WindowMenuButtonPosition />
                    <ColorButtonPosition />
                    <ReactImgui.SameLine>
                        <ButtonTextAlign />
                        <HelpMarker text="Alignment applies when a button is larger than its text content." />
                    </ReactImgui.SameLine>
                    <ReactImgui.SameLine>
                        <SelectableTextAlign />
                        <HelpMarker text="Alignment applies when a selectable is larger than its text content." />
                    </ReactImgui.SameLine>
                    <SeparatorTextBorderSize />
                    <SeparatorTextAlign />
                    <SeparatorTextPadding />
                    <LogSliderDeadzone />
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
        </>
    );
};

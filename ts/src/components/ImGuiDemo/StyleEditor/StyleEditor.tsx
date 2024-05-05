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
                    {/* 
                    ImGui::SeparatorText("Widgets");
                    ImGui::SliderFloat2("WindowTitleAlign", (float*)&style.WindowTitleAlign, 0.0f, 1.0f, "%.2f");
                    int window_menu_button_position = style.WindowMenuButtonPosition + 1;
                    if (ImGui::Combo("WindowMenuButtonPosition", (int*)&window_menu_button_position, "None\0Left\0Right\0"))
                        style.WindowMenuButtonPosition = window_menu_button_position - 1;
                    ImGui::Combo("ColorButtonPosition", (int*)&style.ColorButtonPosition, "Left\0Right\0");
                    ImGui::SliderFloat2("ButtonTextAlign", (float*)&style.ButtonTextAlign, 0.0f, 1.0f, "%.2f");
                    ImGui::SameLine(); HelpMarker("Alignment applies when a button is larger than its text content.");
                    ImGui::SliderFloat2("SelectableTextAlign", (float*)&style.SelectableTextAlign, 0.0f, 1.0f, "%.2f");
                    ImGui::SameLine(); HelpMarker("Alignment applies when a selectable is larger than its text content.");
                    ImGui::SliderFloat("SeparatorTextBorderSize", &style.SeparatorTextBorderSize, 0.0f, 10.0f, "%.0f");
                    ImGui::SliderFloat2("SeparatorTextAlign", (float*)&style.SeparatorTextAlign, 0.0f, 1.0f, "%.2f");
                    ImGui::SliderFloat2("SeparatorTextPadding", (float*)&style.SeparatorTextPadding, 0.0f, 40.0f, "%.0f");
                    ImGui::SliderFloat("LogSliderDeadzone", &style.LogSliderDeadzone, 0.0f, 12.0f, "%.0f");
                     */}
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

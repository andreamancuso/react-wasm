import { WindowTitleAlign } from "src/components/ImGuiDemo/StyleEditor/Sizes/Widgets/WindowTitleAlign/WindowTitleAlign";
import { WindowMenuButtonPosition } from "src/components/ImGuiDemo/StyleEditor/Sizes/Widgets/WindowMenuButtonPosition/WindowMenuButtonPosition";
import { ColorButtonPosition } from "src/components/ImGuiDemo/StyleEditor/Sizes/Widgets/ColorButtonPosition/ColorButtonPosition";
import { ButtonTextAlign } from "src/components/ImGuiDemo/StyleEditor/Sizes/Widgets/ButtonTextAlign/ButtonTextAlign";
import { SelectableTextAlign } from "src/components/ImGuiDemo/StyleEditor/Sizes/Widgets/SelectableTextAlign/SelectableTextAlign";
import { SeparatorTextBorderSize } from "src/components/ImGuiDemo/StyleEditor/Sizes/Widgets/SeparatorTextBorderSize/SeparatorTextBorderSize";
import { SeparatorTextAlign } from "src/components/ImGuiDemo/StyleEditor/Sizes/Widgets/SeparatorTextAlign/SeparatorTextAlign";
import { SeparatorTextPadding } from "src/components/ImGuiDemo/StyleEditor/Sizes/Widgets/SeparatorTextPadding/SeparatorTextPadding";
import { LogSliderDeadzone } from "src/components/ImGuiDemo/StyleEditor/Sizes/Widgets/LogSliderDeadzone/LogSliderDeadzone";
import { ReactImgui } from "src/lib/components/ReactImgui/components";
import { HelpMarker } from "src/components/ImGuiDemo/HelpMarker/HelpMarker";

export const Widgets = () => {
    return (
        <>
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
        </>
    );
};

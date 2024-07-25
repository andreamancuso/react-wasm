import { WindowTitleAlign } from "./WindowTitleAlign/WindowTitleAlign";
import { WindowMenuButtonPosition } from "./WindowMenuButtonPosition/WindowMenuButtonPosition";
import { ColorButtonPosition } from "./ColorButtonPosition/ColorButtonPosition";
import { ButtonTextAlign } from "./ButtonTextAlign/ButtonTextAlign";
import { SelectableTextAlign } from "./SelectableTextAlign/SelectableTextAlign";
import { SeparatorTextBorderSize } from "./SeparatorTextBorderSize/SeparatorTextBorderSize";
import { SeparatorTextAlign } from "./SeparatorTextAlign/SeparatorTextAlign";
import { SeparatorTextPadding } from "./SeparatorTextPadding/SeparatorTextPadding";
import { LogSliderDeadzone } from "./LogSliderDeadzone/LogSliderDeadzone";
import { ReactImgui } from "src/lib/components/ReactImgui/components";
import { HelpMarker } from "../../../HelpMarker/HelpMarker";

export const Widgets = () => {
    return (
        <>
            <WindowTitleAlign />
            <WindowMenuButtonPosition />
            <ColorButtonPosition />
            <ReactImgui.Node style={{ flexDirection: "row" }}>
                <ButtonTextAlign />
                <HelpMarker text="Alignment applies when a button is larger than its text content." />
            </ReactImgui.Node>
            <ReactImgui.Node style={{ flexDirection: "row" }}>
                <SelectableTextAlign />
                <HelpMarker text="Alignment applies when a selectable is larger than its text content." />
            </ReactImgui.Node>
            <SeparatorTextBorderSize />
            <SeparatorTextAlign />
            <SeparatorTextPadding />
            <LogSliderDeadzone />
        </>
    );
};

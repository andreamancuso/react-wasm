import { GrabMinSize } from "src/components/ImGuiDemo/StyleEditor/Sizes/Main/GrabMinSize/GrabMinSize";
import { ItemInnerSpacing } from "src/components/ImGuiDemo/StyleEditor/Sizes/Main/ItemInnerSpacing/ItemInnerSpacing";
import { FramePadding } from "src/components/ImGuiDemo/StyleEditor/Sizes/Main/FramePadding/FramePadding";
import { IndentSpacing } from "src/components/ImGuiDemo/StyleEditor/Sizes/Main/IndentSpacing/IndentSpacing";
import { ItemSpacing } from "src/components/ImGuiDemo/StyleEditor/Sizes/Main/ItemSpacing/ItemSpacing";
import { ScrollbarSize } from "src/components/ImGuiDemo/StyleEditor/Sizes/Main/ScrollbarSize/ScrollbarSize";
import { TouchExtraPadding } from "src/components/ImGuiDemo/StyleEditor/Sizes/Main/TouchExtraPadding/TouchExtraPadding";
import { WindowPadding } from "src/components/ImGuiDemo/StyleEditor/Sizes/Main/WindowPadding/WindowPadding";

export const Main = () => {
    return (
        <>
            <WindowPadding />
            <FramePadding />
            <ItemSpacing />
            <ItemInnerSpacing />
            <TouchExtraPadding />
            <IndentSpacing />
            <ScrollbarSize />
            <GrabMinSize />
        </>
    );
};

import { GrabMinSize } from "./GrabMinSize/GrabMinSize";
import { ItemInnerSpacing } from "./ItemInnerSpacing/ItemInnerSpacing";
import { FramePadding } from "./FramePadding/FramePadding";
import { IndentSpacing } from "./IndentSpacing/IndentSpacing";
import { ItemSpacing } from "./ItemSpacing/ItemSpacing";
import { ScrollbarSize } from "./ScrollbarSize/ScrollbarSize";
import { TouchExtraPadding } from "./TouchExtraPadding/TouchExtraPadding";
import { WindowPadding } from "./WindowPadding/WindowPadding";

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

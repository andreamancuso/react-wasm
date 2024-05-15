import { WindowBorderSize } from "./WindowBorderSize/WindowBorderSize";
import { ChildBorderSize } from "./ChildBorderSize/ChildBorderSize";
import { PopupBorderSize } from "./PopupBorderSize/PopupBorderSize";
import { FrameBorderSize } from "./FrameBorderSize/FrameBorderSize";
import { TabBorderSize } from "./TabBorderSize/TabBorderSize";
import { TabBarBorderSize } from "./TabBarBorderSize/TabBarBorderSize";

export const Borders = () => {
    return (
        <>
            <WindowBorderSize />
            <ChildBorderSize />
            <PopupBorderSize />
            <FrameBorderSize />
            <TabBorderSize />
            <TabBarBorderSize />
        </>
    );
};

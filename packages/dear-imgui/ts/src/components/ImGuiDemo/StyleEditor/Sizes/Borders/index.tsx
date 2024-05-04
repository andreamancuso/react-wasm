import { WindowBorderSize } from "src/components/ImGuiDemo/StyleEditor/Sizes/Borders/WindowBorderSize/WindowBorderSize";
import { ChildBorderSize } from "src/components/ImGuiDemo/StyleEditor/Sizes/Borders/ChildBorderSize/ChildBorderSize";
import { PopupBorderSize } from "src/components/ImGuiDemo/StyleEditor/Sizes/Borders/PopupBorderSize/PopupBorderSize";
import { FrameBorderSize } from "src/components/ImGuiDemo/StyleEditor/Sizes/Borders/FrameBorderSize/FrameBorderSize";
import { TabBorderSize } from "src/components/ImGuiDemo/StyleEditor/Sizes/Borders/TabBorderSize/TabBorderSize";
import { TabBarBorderSize } from "src/components/ImGuiDemo/StyleEditor/Sizes/Borders/TabBarBorderSize/TabBarBorderSize";

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

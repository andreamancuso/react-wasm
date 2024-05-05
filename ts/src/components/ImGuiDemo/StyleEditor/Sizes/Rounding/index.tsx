import { WindowRounding } from "src/components/ImGuiDemo/StyleEditor/Sizes/Rounding/WindowRounding/WindowRounding";
import { ChildRounding } from "src/components/ImGuiDemo/StyleEditor/Sizes/Rounding/ChildRounding/ChildRounding";
import { PopupRounding } from "src/components/ImGuiDemo/StyleEditor/Sizes/Rounding/PopupRounding/PopupRounding";
import { ScrollbarRounding } from "src/components/ImGuiDemo/StyleEditor/Sizes/Rounding/ScrollbarRounding/ScrollbarRounding";
import { GrabRounding } from "src/components/ImGuiDemo/StyleEditor/Sizes/Rounding/GrabRounding/GrabRounding";
import { TabRounding } from "src/components/ImGuiDemo/StyleEditor/Sizes/Rounding/TabRounding/TabRounding";
import { FrameRounding } from "src/components/ImGuiDemo/StyleEditor/Sizes/Rounding/FrameRounding/FrameRounding";

export const Rounding = () => {
    return (
        <>
            <WindowRounding />
            <ChildRounding />
            <FrameRounding />
            <PopupRounding />
            <ScrollbarRounding />
            <GrabRounding />
            <TabRounding />
        </>
    );
};

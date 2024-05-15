import { WindowRounding } from "./WindowRounding/WindowRounding";
import { ChildRounding } from "./ChildRounding/ChildRounding";
import { PopupRounding } from "./PopupRounding/PopupRounding";
import { ScrollbarRounding } from "./ScrollbarRounding/ScrollbarRounding";
import { GrabRounding } from "./GrabRounding/GrabRounding";
import { TabRounding } from "./TabRounding/TabRounding";
import { FrameRounding } from "./FrameRounding/FrameRounding";

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

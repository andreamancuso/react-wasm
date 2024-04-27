import { ReactImgui } from "../../ReactImgui/components";
import { BorderControl } from "../BorderControl/BorderControl";
import { FontSelector } from "../FontSelector/FontSelector";
import { FrameRounding } from "../FrameRounding/FrameRounding";
import { HelpMarker } from "../HelpMarker/HelpMarker";
import { StyleSelector } from "../StyleSelector/StyleSelector";
import { WindowPadding } from "../WindowPadding/WindowPadding";

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

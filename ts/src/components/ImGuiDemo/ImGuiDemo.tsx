import { useCallback, useState } from "react";
import { HelpMarker } from "./HelpMarker/HelpMarker";
import { ReactImgui } from "../ReactImgui/components";
import { UserGuide } from "./UserGuide/UserGuide";
import { StyleEditor } from "./StyleEditor/StyleEditor";

export const ImGuiDemo = () => {
    const [text, setText] = useState("Hello, world!");

    const handleInputTextChanged = useCallback((value: string) => {
        setText(value);
    }, []);

    return (
        <>
            <ReactImgui.SameLine>
                <ReactImgui.InputText defaultValue={text} onChange={handleInputTextChanged} />
                <ReactImgui.UnformattedText text={text} />
            </ReactImgui.SameLine>
            <ReactImgui.UnformattedText text="dear imgui says hello!" />

            <ReactImgui.CollapsingHeader label="Help">
                <ReactImgui.SeparatorText label="ABOUT THIS DEMO:" />
                <ReactImgui.BulletText text="Sections below are demonstrating many aspects of the library." />
                <ReactImgui.BulletText text='The "Examples" menu above leads to more demo contents.' />
                <ReactImgui.BulletText
                    text={`The "Tools" menu above gives access to: About Box, Style Editor,\nand Metrics/Debugger (general purpose Dear ImGui debugging tool).`}
                />
                <ReactImgui.SeparatorText label="PROGRAMMER GUIDE:" />
                <ReactImgui.BulletText text="See the ShowDemoWindow() code in imgui_demo.cpp. <- you are here!" />
                <ReactImgui.BulletText text="See comments in imgui.cpp." />
                <ReactImgui.BulletText text="See example applications in the examples/ folder." />
                <ReactImgui.BulletText text="Read the FAQ at https://www.dearimgui.com/faq/" />
                <ReactImgui.BulletText text="Set 'io.ConfigFlags |= NavEnableKeyboard' for keyboard controls." />
                <ReactImgui.BulletText text="Set 'io.ConfigFlags |= NavEnableGamepad' for gamepad controls." />

                <ReactImgui.SeparatorText label="USER GUIDE:" />
                <UserGuide />
            </ReactImgui.CollapsingHeader>
            <ReactImgui.CollapsingHeader label="Configuration">
                <ReactImgui.TreeNode label="Configuration">
                    <ReactImgui.UnformattedText text="Coming soon!" />
                </ReactImgui.TreeNode>
                <ReactImgui.TreeNode label="Backend Flags">
                    <HelpMarker
                        text={`Those flags are set by the backends (imgui_impl_xxx files) to specify their capabilities.\nHere we expose them as read-only fields to avoid breaking interactions with your backend.`}
                    />
                    <ReactImgui.UnformattedText text="Coming soon!" />
                </ReactImgui.TreeNode>
                <ReactImgui.TreeNode label="Style">
                    <HelpMarker text="The same contents can be accessed in 'Tools->Style Editor' or by calling the ShowStyleEditor() function." />
                    <StyleEditor />
                </ReactImgui.TreeNode>
            </ReactImgui.CollapsingHeader>
        </>
    );
};

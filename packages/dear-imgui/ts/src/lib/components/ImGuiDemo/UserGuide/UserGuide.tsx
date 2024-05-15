import { ReactImgui } from "../../../components/ReactImgui/components";

export const UserGuide = () => (
    <>
        <ReactImgui.BulletText text="Double-click on title bar to collapse window." />
        <ReactImgui.BulletText
            text={`Click and drag on lower corner to resize window\n(double-click to auto fit window to its contents).`}
        />
        <ReactImgui.BulletText text="CTRL+Click on a slider or drag box to input value as text." />
        <ReactImgui.BulletText text="TAB/SHIFT+TAB to cycle through keyboard editable fields." />
        <ReactImgui.BulletText text="CTRL+Tab to select a window." />
        {/* if (io.FontAllowUserScaling) */}
        {/* ImGui::BulletText("CTRL+Mouse Wheel to zoom window contents."); */}
        <ReactImgui.BulletText text={`While inputing text:\n`} />
        <ReactImgui.Indent>
            <ReactImgui.BulletText text="CTRL+Left/Right to word jump." />
            <ReactImgui.BulletText text="CTRL+A or double-click to select all." />
            <ReactImgui.BulletText text="CTRL+X/C/V to use clipboard cut/copy/paste." />
            <ReactImgui.BulletText text="CTRL+Z,CTRL+Y to undo/redo." />
            <ReactImgui.BulletText text="ESCAPE to revert." />
        </ReactImgui.Indent>
        <ReactImgui.BulletText text="With keyboard navigation enabled:" />
        <ReactImgui.Indent>
            <ReactImgui.BulletText text="Arrow keys to navigate." />
            <ReactImgui.BulletText text="Space to activate a widget." />
            <ReactImgui.BulletText text="Return to input text into a widget." />
            <ReactImgui.BulletText text="Escape to deactivate a widget, close popup, exit child window." />
            <ReactImgui.BulletText text="Alt to jump to the menu layer of a window." />
        </ReactImgui.Indent>
    </>
);

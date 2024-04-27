import { ReactImgui } from "../../ReactImgui/components";

type HelpMarkerProps = {
    text: string;
};

export const HelpMarker = ({ text }: HelpMarkerProps) => (
    <ReactImgui.Fragment>
        <ReactImgui.TextDisabled text="(?)" />
        <ReactImgui.ItemTooltip>
            <ReactImgui.TextWrap width={35 * 12}>
                <ReactImgui.UnformattedText text={text} />
            </ReactImgui.TextWrap>
        </ReactImgui.ItemTooltip>
    </ReactImgui.Fragment>
);

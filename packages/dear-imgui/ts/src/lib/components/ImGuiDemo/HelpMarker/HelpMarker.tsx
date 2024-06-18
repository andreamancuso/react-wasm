import { ReactImgui } from "../../../components/ReactImgui/components";

type HelpMarkerProps = {
    text: string;
};

export const HelpMarker = ({ text }: HelpMarkerProps) => (
    <ReactImgui.Node>
        <ReactImgui.DisabledText text="(?)" />
        <ReactImgui.ItemTooltip>
            <ReactImgui.TextWrap width={35 * 12}>
                <ReactImgui.UnformattedText text={text} />
            </ReactImgui.TextWrap>
        </ReactImgui.ItemTooltip>
    </ReactImgui.Node>
);

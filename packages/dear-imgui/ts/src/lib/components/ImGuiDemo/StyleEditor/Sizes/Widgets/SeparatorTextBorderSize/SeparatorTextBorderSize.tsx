import { useCallback, useState } from "react";
import { ReactImgui } from "../../../../../../components/ReactImgui/components";

export const SeparatorTextBorderSize = () => {
    const [value, setValue] = useState(0);

    const handleValueChanged = useCallback((event: any) => {
        if (event.nativeEvent) {
            setValue(event.nativeEvent.value);
        }
    }, []);

    return (
        <ReactImgui.SameLine>
            <ReactImgui.Slider
                label="SeparatorTextBorderSize"
                onChange={handleValueChanged}
                min={0}
                max={10}
            />
            <ReactImgui.UnformattedText text={`[${value}]`} />
        </ReactImgui.SameLine>
    );
};

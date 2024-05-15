import { useCallback, useState } from "react";
import { ReactImgui } from "../../../../../../components/ReactImgui/components";

export const ChildBorderSize = () => {
    const [value, setValue] = useState(0);

    const handleValueChanged = useCallback((event: any) => {
        if (event.nativeEvent) {
            setValue(event.nativeEvent.value);
        }
    }, []);

    return (
        <ReactImgui.SameLine>
            <ReactImgui.Slider
                label="ChildBorderSize"
                min={0}
                max={1}
                onChange={handleValueChanged}
            />
            <ReactImgui.UnformattedText text={`(${value})`} />
        </ReactImgui.SameLine>
    );
};

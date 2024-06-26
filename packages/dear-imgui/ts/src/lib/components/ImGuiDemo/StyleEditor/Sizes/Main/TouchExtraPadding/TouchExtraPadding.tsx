import { useCallback, useState } from "react";
import { ReactImgui } from "../../../../../../components/ReactImgui/components";

export const TouchExtraPadding = () => {
    const [value, setValue] = useState<[number, number]>([0, 0]);

    const handleValueChanged = useCallback((event: any) => {
        if (event.nativeEvent) {
            setValue([event.nativeEvent.values[0], event.nativeEvent.values[1]]);
        }
    }, []);

    return (
        <ReactImgui.SameLine>
            <ReactImgui.MultiSlider
                label="TouchExtraPadding"
                onChange={handleValueChanged}
                numValues={2}
                min={0}
                max={10}
            />
            {value && <ReactImgui.UnformattedText text={`[${value[0]},${value[1]}]`} />}
        </ReactImgui.SameLine>
    );
};

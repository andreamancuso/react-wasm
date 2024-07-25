import { useCallback, useState } from "react";
import { ReactImgui } from "../../../../../../components/ReactImgui/components";

export const FramePadding = () => {
    const [value, setValue] = useState<[number, number]>([0, 0]);

    const handleValueChanged = useCallback((event: any) => {
        if (event.nativeEvent) {
            setValue([event.nativeEvent.values[0], event.nativeEvent.values[1]]);
        }
    }, []);

    return (
        <ReactImgui.Node style={{ flexDirection: "row" }}>
            <ReactImgui.MultiSlider
                label="FramePadding"
                onChange={handleValueChanged}
                numValues={2}
                min={0}
                max={20}
            />
            {value && <ReactImgui.UnformattedText text={`[${value[0]},${value[1]}]`} />}
        </ReactImgui.Node>
    );
};

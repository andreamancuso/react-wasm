import { useCallback, useState } from "react";
import { ReactImgui } from "../../../../../../components/ReactImgui/components";

export const FrameBorderSize = () => {
    const [value, setValue] = useState(0);

    const handleValueChanged = useCallback((event: any) => {
        if (event.nativeEvent) {
            setValue(event.nativeEvent.value);
        }
    }, []);

    return (
        <ReactImgui.Node style={{ flexDirection: "row" }}>
            <ReactImgui.Slider
                label="FrameBorderSize"
                min={0}
                max={1}
                onChange={handleValueChanged}
                sliderType="default"
            />
            <ReactImgui.UnformattedText text={`(${value})`} />
        </ReactImgui.Node>
    );
};

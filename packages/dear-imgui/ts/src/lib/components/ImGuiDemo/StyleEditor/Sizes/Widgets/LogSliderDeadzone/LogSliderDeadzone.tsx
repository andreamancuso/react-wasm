import { useCallback, useState } from "react";
import { ReactImgui } from "../../../../../../components/ReactImgui/components";

export const LogSliderDeadzone = () => {
    const [value, setValue] = useState(0);

    const handleValueChanged = useCallback((event: any) => {
        if (event.nativeEvent) {
            setValue(event.nativeEvent.value);
        }
    }, []);

    return (
        <ReactImgui.Node style={{ flexDirection: "row" }}>
            <ReactImgui.Slider
                label="LogSliderDeadzone"
                onChange={handleValueChanged}
                min={0}
                max={12}
            />
            <ReactImgui.UnformattedText text={`[${value}]`} />
        </ReactImgui.Node>
    );
};

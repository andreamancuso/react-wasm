import { useCallback, useState } from "react";
import { ReactImgui } from "../../../../../../components/ReactImgui/components";

export const IndentSpacing = () => {
    const [value, setValue] = useState(0);

    const handleValueChanged = useCallback((event: any) => {
        if (event.nativeEvent) {
            setValue(event.nativeEvent.value);
        }
    }, []);

    return (
        <ReactImgui.Node style={{ flexDirection: "row" }}>
            <ReactImgui.Slider
                label="IndentSpacing"
                onChange={handleValueChanged}
                min={0}
                max={30}
            />
            <ReactImgui.UnformattedText text={`[${value}]`} />
        </ReactImgui.Node>
    );
};

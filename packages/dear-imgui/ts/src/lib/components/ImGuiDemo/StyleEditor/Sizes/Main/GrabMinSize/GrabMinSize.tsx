import { useCallback, useState } from "react";
import { ReactImgui } from "../../../../../../components/ReactImgui/components";

export const GrabMinSize = () => {
    const [value, setValue] = useState(0);

    const handleValueChanged = useCallback((event: any) => {
        if (event.nativeEvent) {
            setValue(event.nativeEvent.value);
        }
    }, []);

    return (
        <ReactImgui.Node style={{ flexDirection: "row" }}>
            <ReactImgui.Slider label="GrabMinSize" onChange={handleValueChanged} min={1} max={20} />
            <ReactImgui.UnformattedText text={`[${value}]`} />
        </ReactImgui.Node>
    );
};

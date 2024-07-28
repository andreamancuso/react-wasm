import { useCallback, useState } from "react";
import { ReactImgui } from "../../../../components/ReactImgui/components";

const options = ["Dark", "Light", "Classic"];

export const StyleSelector = () => {
    const [selectedStyleIndex, setSelectedStyleIndex] = useState();

    const handleStyleChanged = useCallback((event: any) => {
        if (event?.nativeEvent) {
            setSelectedStyleIndex(event?.nativeEvent.value);
        }
    }, []);

    return (
        <ReactImgui.Node style={{ flexDirection: "row", alignItems: "center", gap: { column: 5 } }}>
            <ReactImgui.Combo
                placeholder="Select style..."
                options={options}
                onChange={handleStyleChanged}
            />
            <ReactImgui.UnformattedText text={`Selected style index: ${selectedStyleIndex}`} />
        </ReactImgui.Node>
    );
};

import { useCallback, useState } from "react";
import { ReactImgui } from "../../../../components/ReactImgui/components";

const options = [
    {
        value: 0,
        label: "Dark",
    },
    {
        value: 1,
        label: "Light",
    },
    {
        value: 2,
        label: "Classic",
    },
];

export const StyleSelector = () => {
    const [selectedStyleIndex, setSelectedStyleIndex] = useState(1);

    const handleStyleChanged = useCallback((event: any) => {
        if (event?.nativeEvent) {
            setSelectedStyleIndex(event?.nativeEvent.value);
        }
    }, []);

    return (
        <ReactImgui.Node style={{ flexDirection: "row", alignItems: "center", gap: { column: 5 } }}>
            <ReactImgui.Combo
                label="Colors"
                options={options}
                onChange={handleStyleChanged}
                defaultValue={1}
            />
            <ReactImgui.UnformattedText text={`Selected style index: ${selectedStyleIndex}`} />
        </ReactImgui.Node>
    );
};

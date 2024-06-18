import { useCallback, useMemo, useState } from "react";
import { ReactImgui } from "../../../../../../components/ReactImgui/components";

export const WindowMenuButtonPosition = () => {
    const [selectedIndex, setSelectedIndex] = useState(0);
    const options = useMemo(
        () => [
            { value: 0, label: "None" },
            { value: 1, label: "Left" },
            { value: 2, label: "Right" },
        ],
        [],
    );

    const handleSelectedIndexChanged = useCallback((event: any) => {
        if (event?.nativeEvent) {
            setSelectedIndex(event?.nativeEvent.value);
        }
    }, []);

    return (
        <ReactImgui.Node style={{ flexDirection: "row" }}>
            <ReactImgui.Combo
                label="WindowMenuButtonPosition"
                options={options}
                onChange={handleSelectedIndexChanged}
                defaultValue={selectedIndex}
            />
            <ReactImgui.UnformattedText text={`Selected index: ${selectedIndex}`} />
        </ReactImgui.Node>
    );
};

import { useCallback, useMemo, useState } from "react";
import { ReactImgui } from "src/lib/components/ReactImgui/components";
import { useWidgetRegistrationService } from "src/lib/hooks/useWidgetRegistrationService";

export const ColorButtonPosition = () => {
    const service = useWidgetRegistrationService();
    const [selectedIndex, setSelectedIndex] = useState(0);
    const options = useMemo(
        () => [
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
        <ReactImgui.SameLine>
            <ReactImgui.Combo
                label="ColorButtonPosition"
                options={options}
                onChange={handleSelectedIndexChanged}
                defaultValue={selectedIndex}
            />
            <ReactImgui.UnformattedText text={`Selected index: ${selectedIndex}`} />
        </ReactImgui.SameLine>
    );
};

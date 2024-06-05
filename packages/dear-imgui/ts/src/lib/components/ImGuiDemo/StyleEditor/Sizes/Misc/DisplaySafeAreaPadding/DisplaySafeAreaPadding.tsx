import { useCallback, useState } from "react";
import { ReactImgui } from "../../../../../../components/ReactImgui/components";
import { HelpMarker } from "../../../../HelpMarker/HelpMarker";

export const DisplaySafeAreaPadding = () => {
    const [value, setValue] = useState<[number, number]>([0, 0]);

    const handleValueChanged = useCallback((event: any) => {
        if (event.nativeEvent) {
            setValue([event.nativeEvent.values[0], event.nativeEvent.values[1]]);
        }
    }, []);

    return (
        <ReactImgui.SameLine>
            <ReactImgui.MultiSlider
                label="DisplaySafeAreaPadding"
                onChange={handleValueChanged}
                numValues={2}
                min={0}
                max={30}
            />
            <HelpMarker text="Adjust if you cannot see the edges of your screen (e.g. on a TV where scaling has not been configured)." />
            {value && <ReactImgui.UnformattedText text={`[${value[0]},${value[1]}]`} />}
        </ReactImgui.SameLine>
    );
};

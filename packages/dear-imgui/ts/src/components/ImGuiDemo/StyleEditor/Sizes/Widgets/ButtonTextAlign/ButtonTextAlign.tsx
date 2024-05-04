import { useCallback, useState } from "react";
import { ReactImgui } from "src/lib/components/ReactImgui/components";
import { useWidgetRegistrationService } from "src/lib/hooks/useWidgetRegistrationService";
import { Primitive } from "src/lib/components/ReactImgui/types";

export const ButtonTextAlign = () => {
    const service = useWidgetRegistrationService();
    const [value, setValue] = useState<[number, number]>([9, 9]);

    const handleValueChanged = useCallback((event: any) => {
        if (event.nativeEvent) {
            setValue([event.nativeEvent.values[0], event.nativeEvent.values[1]]);
        }
    }, []);

    return (
        <ReactImgui.SameLine>
            <ReactImgui.MultiSlider
                label="ButtonTextAlign"
                onChange={handleValueChanged}
                numValues={2}
                decimalDigits={2}
                min={0}
                max={1}
            />
            {value && <ReactImgui.UnformattedText text={`[${value[0]},${value[1]}]`} />}
        </ReactImgui.SameLine>
    );
};

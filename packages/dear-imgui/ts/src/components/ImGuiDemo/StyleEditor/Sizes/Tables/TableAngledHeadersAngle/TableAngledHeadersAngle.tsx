import { useCallback, useMemo, useState } from "react";
import { ReactImgui } from "src/lib/components/ReactImgui/components";
import { useWidgetRegistrationService } from "src/lib/hooks/useWidgetRegistrationService";

export const TableAngledHeadersAngle = () => {
    const service = useWidgetRegistrationService();
    const [value, setValue] = useState(0);

    const handleValueChanged = useCallback((event: any) => {
        if (event.nativeEvent) {
            setValue((event.nativeEvent.value * 360) / (2 * Math.PI));
        }
    }, []);

    return (
        <ReactImgui.SameLine>
            <ReactImgui.Slider
                label="TableAngledHeadersAngle"
                min={-50} // degrees
                max={50} // degrees
                onChange={handleValueChanged}
                sliderType="angle" // value is in radians
            />
            <ReactImgui.UnformattedText text={`(${value})`} />
        </ReactImgui.SameLine>
    );
};

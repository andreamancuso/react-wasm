import { useCallback, useMemo, useState } from "react";
import { ReactImgui } from "src/components/ReactImgui/components";
import { useWidgetRegistrationService } from "src/hooks/useWidgetRegistrationService";

export const TableAngledHeadersAngle = () => {
    const service = useWidgetRegistrationService();
    const [value, setValue] = useState(0);

    const handleValueChanged = useCallback((valueInRadians: number) => {
        setValue((valueInRadians * 360) / (2 * Math.PI));
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

import { useCallback, useMemo, useState } from "react";
import { ReactImgui } from "src/components/ReactImgui/components";
import { useWidgetRegistrationService } from "src/hooks/useWidgetRegistrationService";

export const FrameBorderSize = () => {
    const service = useWidgetRegistrationService();
    const [value, setValue] = useState(0);

    const handleValueChanged = useCallback((event: any) => {
        if (event.nativeEvent) {
            setValue(event.nativeEvent.value);
        }
    }, []);

    return (
        <ReactImgui.SameLine>
            <ReactImgui.Slider
                label="FrameBorderSize"
                min={0}
                max={1}
                onChange={handleValueChanged}
            />
            <ReactImgui.UnformattedText text={`(${value})`} />
        </ReactImgui.SameLine>
    );
};

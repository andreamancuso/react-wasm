import { useCallback, useMemo, useState } from "react";
import { ReactImgui } from "src/lib/components/ReactImgui/components";
import { useWidgetRegistrationService } from "src/lib/hooks/useWidgetRegistrationService";

export const PopupRounding = () => {
    const service = useWidgetRegistrationService();
    const [value, setValue] = useState(0);

    const handleValueChanged = useCallback((event: any) => {
        if (event?.nativeEvent) {
            setValue(event?.nativeEvent.value);
        }
    }, []);

    return (
        <ReactImgui.SameLine>
            <ReactImgui.Slider
                label="PopupRounding"
                min={0}
                max={12}
                onChange={handleValueChanged}
            />
            <ReactImgui.UnformattedText text={`(${value})`} />
        </ReactImgui.SameLine>
    );
};
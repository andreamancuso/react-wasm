import { useCallback, useState } from "react";
import { ReactImgui } from "src/components/ReactImgui/components";
import { useWidgetRegistrationService } from "src/hooks/useWidgetRegistrationService";

export const ScrollbarSize = () => {
    const service = useWidgetRegistrationService();
    const [value, setValue] = useState(0);

    // todo: remove casting
    const handleValueChanged = useCallback((value: number) => {
        setValue(value);
    }, []);

    return (
        <ReactImgui.SameLine>
            <ReactImgui.Slider
                label="ScrollbarSize"
                onChange={handleValueChanged}
                min={1}
                max={20}
            />
            <ReactImgui.UnformattedText text={`[${value}]`} />
        </ReactImgui.SameLine>
    );
};

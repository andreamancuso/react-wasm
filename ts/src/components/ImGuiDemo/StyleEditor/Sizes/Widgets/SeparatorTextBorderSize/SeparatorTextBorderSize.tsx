import { useCallback, useState } from "react";
import { ReactImgui } from "src/components/ReactImgui/components";
import { useWidgetRegistrationService } from "src/hooks/useWidgetRegistrationService";

export const SeparatorTextBorderSize = () => {
    const service = useWidgetRegistrationService();
    const [value, setValue] = useState(0);

    // todo: remove casting
    const handleValueChanged = useCallback((value: number) => {
        setValue(value);
    }, []);

    return (
        <ReactImgui.SameLine>
            <ReactImgui.Slider
                label="SeparatorTextBorderSize"
                onChange={handleValueChanged}
                min={0}
                max={10}
            />
            <ReactImgui.UnformattedText text={`[${value}]`} />
        </ReactImgui.SameLine>
    );
};

import { useCallback, useState } from "react";
import { ReactImgui } from "../../../../../ReactImgui/components";
import { useWidgetRegistrationService } from "../../../../../../hooks/useWidgetRegistrationService";

export const GrabMinSize = () => {
    const service = useWidgetRegistrationService();
    const [value, setValue] = useState(0);

    // todo: remove casting
    const handleValueChanged = useCallback((value: number) => {
        setValue(value);
    }, []);

    return (
        <ReactImgui.SameLine>
            <ReactImgui.Slider label="GrabMinSize" onChange={handleValueChanged} min={1} max={20} />
            <ReactImgui.UnformattedText text={`[${value}]`} />
        </ReactImgui.SameLine>
    );
};

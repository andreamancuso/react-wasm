import { useCallback, useMemo, useState } from "react";
import { ReactImgui } from "../../../../../ReactImgui/components";
import { useWidgetRegistrationService } from "../../../../../../hooks/useWidgetRegistrationService";

export const CellPadding = () => {
    const service = useWidgetRegistrationService();
    const [value, setValue] = useState(0);

    const handleValueChanged = useCallback((value: number) => {
        setValue(value);
    }, []);

    return (
        <ReactImgui.SameLine>
            <ReactImgui.Slider label="CellPadding" min={0} max={20} onChange={handleValueChanged} />
            <ReactImgui.UnformattedText text={`(${value})`} />
        </ReactImgui.SameLine>
    );
};

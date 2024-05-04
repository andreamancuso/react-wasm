import { useCallback, useState } from "react";
import { ReactImgui } from "../../ReactImgui/components";
import { useWidgetRegistrationService } from "../../../hooks/useWidgetRegistrationService";
import { Primitive } from "../../ReactImgui/types";

export const WindowPadding = () => {
    const service = useWidgetRegistrationService();
    const [value, setValue] = useState<[number, number]>([9, 9]);

    // todo: remove casting
    const handleValueChanged = useCallback((...args: Primitive[]) => {
        setValue([args[0] as number, args[1] as number]);
    }, []);

    return (
        <ReactImgui.SameLine>
            <ReactImgui.MultiSlider
                label="WindowPadding"
                onChange={handleValueChanged}
                numValues={2}
                min={0}
                max={20}
            />
            {value && <ReactImgui.UnformattedText text={`[${value[0]},${value[1]}]`} />}
        </ReactImgui.SameLine>
    );
};

import { useCallback, useState } from "react";
import { ReactImgui } from "../../ReactImgui/components";
import { useWidgetRegistrationService } from "../../../hooks/useWidgetRegistrationService";

export const BorderControl = () => {
    const service = useWidgetRegistrationService();
    const [windowBorder, setWindowBorder] = useState(true);
    const [frameBorder, setFrameBorder] = useState(false);
    const [popupBorder, setPopupBorder] = useState(true);

    const handleWindowBorderChanged = useCallback((value: boolean) => {
        setWindowBorder(value);
    }, []);

    const handleFrameBorderChanged = useCallback((value: boolean) => {
        setFrameBorder(value);
    }, []);

    const handlePopupBorderChanged = useCallback((value: boolean) => {
        setPopupBorder(value);
    }, []);

    return (
        <ReactImgui.SameLine>
            <ReactImgui.Checkbox
                label="WindowBorder"
                onChange={handleWindowBorderChanged}
                defaultChecked={windowBorder}
            />
            <ReactImgui.Checkbox
                label="FrameBorder"
                onChange={handleFrameBorderChanged}
                defaultChecked={frameBorder}
            />
            <ReactImgui.Checkbox
                label="WindowBorder"
                onChange={handlePopupBorderChanged}
                defaultChecked={popupBorder}
            />
        </ReactImgui.SameLine>
    );
};

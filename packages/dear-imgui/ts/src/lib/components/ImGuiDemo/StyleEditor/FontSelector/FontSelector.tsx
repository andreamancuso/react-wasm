import { useCallback, useMemo, useState } from "react";
import { ReactImgui } from "../../../../components/ReactImgui/components";
import { useWidgetRegistrationService } from "../../../../hooks/useWidgetRegistrationService";

export const FontSelector = () => {
    const service = useWidgetRegistrationService();
    const [selectedFontIndex, setSelectedFontIndex] = useState(0);
    const fonts = useMemo(() => service.getFonts(), []);
    const options = useMemo(() => fonts.map((font, i) => ({ value: i, label: font })), [fonts]);

    const handleFontChanged = useCallback((event: any) => {
        if (event?.nativeEvent) {
            setSelectedFontIndex(event?.nativeEvent.value);
        }
    }, []);

    return (
        <ReactImgui.Node style={{ flexDirection: "row" }}>
            <ReactImgui.Combo
                label="Colors"
                options={options}
                onChange={handleFontChanged}
                defaultValue={selectedFontIndex}
            />
            <ReactImgui.UnformattedText text={`Selected font index: ${selectedFontIndex}`} />
        </ReactImgui.Node>
    );
};

import { useCallback, useState } from "react";
import { ReactImgui } from "../../../ReactImgui/components";

const options = [
    {
        value: 0,
        label: "Dark",
    },
    {
        value: 1,
        label: "Light",
    },
    {
        value: 2,
        label: "Classic",
    },
];

export const StyleSelector = () => {
    const [selectedStyleIndex, setSelectedStyleIndex] = useState(1);

    const handleStyleChanged = useCallback((value: number) => {
        setSelectedStyleIndex(value);
    }, []);

    return (
        <ReactImgui.SameLine>
            <ReactImgui.Combo
                label="Colors"
                options={options}
                onChange={handleStyleChanged}
                defaultValue={1}
            />
            <ReactImgui.UnformattedText text={`Selected style index: ${selectedStyleIndex}`} />
        </ReactImgui.SameLine>
    );
};

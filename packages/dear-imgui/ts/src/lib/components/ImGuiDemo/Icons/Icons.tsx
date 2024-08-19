import React, { useMemo } from "react";
import faIconMap, { faIconKeys } from "src/lib/fa-icons";
import { ReactImgui } from "../../ReactImgui/components";
import RWStyleSheet from "src/lib/stylesheet/stylesheet";

export const Icons = () => {
    const styleSheet = useMemo(
        () =>
            RWStyleSheet.create({
                mainWrapperNode: {
                    width: "100%",
                    height: "auto",
                    flexDirection: "row",
                    flexWrap: "wrap",
                    gap: { row: 5, column: 5 },
                },
                iconWrapperNode: {
                    minWidth: 200,
                    maxWidth: 240,
                    flex: 1,
                    height: 100,
                    borderColor: "#000",
                    borderThickness: 1,
                    alignItems: "center",
                },
                icon: {
                    font: { name: "roboto-regular", size: 48 },
                },
                iconKey: {
                    font: { name: "roboto-mono-regular", size: 16 },
                },
            }),
        [],
    );

    return (
        <ReactImgui.Node style={styleSheet.mainWrapperNode}>
            {Object.keys(faIconMap)
                // .slice(0, 60)
                .map((key) => (
                    <ReactImgui.Node key={key} style={styleSheet.iconWrapperNode}>
                        <ReactImgui.UnformattedText
                            text={faIconMap[key as faIconKeys]}
                            style={styleSheet.icon}
                        />
                        <ReactImgui.UnformattedText text={key} style={styleSheet.iconKey} />
                    </ReactImgui.Node>
                ))}
        </ReactImgui.Node>
    );
};

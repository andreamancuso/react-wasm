import React, { useMemo } from "react";
import { ReactImgui } from "src/lib/components/ReactImgui/components";
import RWStyleSheet from "src/lib/stylesheet/stylesheet";
import { useStore } from "../store";
import { CryptoQuotePrice } from "./CryptoQuotePrice";
import { HelpMarker } from "../../ImGuiDemo/HelpMarker/HelpMarker";

type Props = {};

export const CryptoAssetsList = ({}: Props) => {
    const cryptoAssets = useStore((state) => state.cryptoAssets);

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
                asset: {
                    minWidth: 200,
                    maxWidth: 240,
                    flex: 1,
                    height: "auto",
                    borderColor: "#000",
                    borderThickness: 1,
                    alignItems: "center",
                    padding: { vertical: 5 },
                    gap: { row: 5 },
                },
                symbol: {
                    font: { name: "roboto-regular", size: 24 },
                },
            }),
        [],
    );

    return (
        <ReactImgui.Node style={styleSheet.mainWrapperNode}>
            {cryptoAssets.map((asset) => {
                return (
                    <ReactImgui.Node key={asset.id} style={styleSheet.asset}>
                        <ReactImgui.UnformattedText style={styleSheet.symbol} text={asset.symbol} />
                        {/* <ReactImgui.ItemTooltip>
                            <ReactImgui.UnformattedText text={asset.name} />
                        </ReactImgui.ItemTooltip> */}

                        {/** Fix tooltip! */}
                        {/* <HelpMarker text={asset.name} /> */}

                        <CryptoQuotePrice symbol={asset.symbol} />
                    </ReactImgui.Node>
                );
            })}
        </ReactImgui.Node>
    );
};

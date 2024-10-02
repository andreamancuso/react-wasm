import React, { useCallback, useMemo } from "react";
import { ReactImgui } from "src/lib/components/ReactImgui/components";
import RWStyleSheet from "src/lib/stylesheet/stylesheet";
import { useStore } from "../store";

type Props = {};

/**
 * 
 *  {
    id: '34bfd447-3a3f-4d03-b3f7-3bbbfd57c6ab',
    class: 'crypto',
    exchange: 'CRYPTO',
    symbol: 'SUSHI/USDC',
    name: 'SushiSwap / USD Coin',
    status: 'active',
    tradable: true,
    marginable: false,
    maintenance_margin_requirement: 100,
    margin_requirement_long: '100',
    margin_requirement_short: '100',
    shortable: false,
    easy_to_borrow: false,
    fractionable: true,
    attributes: [],
    min_order_size: '1.803101334',
    min_trade_increment: '0.000000001'
  }
 */

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
                    height: 100,
                    borderColor: "#000",
                    borderThickness: 1,
                    alignItems: "center",
                },
                price: {
                    width: 80,
                    height: 30,
                    borderColor: "#000",
                    borderThickness: 1,
                    alignItems: "center",
                },
            }),
        [],
    );

    return (
        <ReactImgui.Node style={styleSheet.mainWrapperNode}>
            {cryptoAssets.map((asset) => (
                <ReactImgui.Node key={asset.id} style={styleSheet.asset}>
                    {/* <ReactImgui.UnformattedText text={asset.id} /> */}
                    <ReactImgui.UnformattedText text={asset.symbol} />
                    <ReactImgui.UnformattedText text={asset.name} />
                    <ReactImgui.UnformattedText text={asset.status} />

                    <ReactImgui.Node key={asset.id} style={styleSheet.price}>
                        <ReactImgui.UnformattedText text={"55.000000"} />
                    </ReactImgui.Node>
                </ReactImgui.Node>
            ))}
        </ReactImgui.Node>
    );
};

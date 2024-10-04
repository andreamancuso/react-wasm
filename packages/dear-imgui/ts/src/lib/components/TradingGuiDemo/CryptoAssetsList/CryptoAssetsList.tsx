import React, { useEffect, useMemo, useRef } from "react";
import { ReactImgui } from "src/lib/components/ReactImgui/components";
import RWStyleSheet from "src/lib/stylesheet/stylesheet";
import { useStore } from "../store";
import { TableImperativeHandle } from "../../ReactImgui/Table";
import { useDataService } from "../dataServiceContext";

type Props = {};

export const CryptoAssetsList = ({}: Props) => {
    const dataService = useDataService();
    const tableRef = useRef<TableImperativeHandle>(null);
    const cryptoAssets = useStore((state) => state.cryptoAssets);

    const tableColumns = useMemo(
        () => [
            {
                heading: "Symbol",
                fieldId: "symbol",
            },
            {
                heading: "ASK",
                fieldId: "askPrice",
            },
            {
                heading: "Change",
                fieldId: "askChange",
            },
            {
                heading: "BID",
                fieldId: "bidPrice",
            },
            {
                heading: "Change",
                fieldId: "bidChange",
            },
        ],
        [],
    );

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

    useEffect(() => {
        const subscription = dataService
            .getCryptoSnapshots()
            // .pipe(filter((cryptoQuote) => cryptoQuote.S === symbol))
            .subscribe((snapshots) => {
                const data = Object.entries(snapshots).map(([symbol, snapshot]) => ({
                    symbol,
                    askPrice: snapshot.LatestQuote.AskPrice.toFixed(6),
                    bidPrice: snapshot.LatestQuote.BidPrice.toFixed(6),
                    // todo: totally unsure here
                    askChange: (snapshot.LatestQuote.AskPrice - snapshot.DailyBar.Open).toFixed(6),
                    bidChange: (snapshot.LatestQuote.BidPrice - snapshot.DailyBar.Open).toFixed(6),
                }));
                data.sort((a, b) => {
                    if (a.symbol < b.symbol) {
                        return -1;
                    } else if (a.symbol > b.symbol) {
                        return 1;
                    }

                    return 0;
                });

                tableRef.current?.setTableData(data);
            });

        return () => {
            subscription.unsubscribe();
        };
    }, [dataService]);

    return <ReactImgui.Table ref={tableRef} columns={tableColumns} />;
};

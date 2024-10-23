import React, { useEffect, useRef } from "react";
import { map } from "rxjs";
import { useDataService } from "../dataServiceContext";
import { PlotCandlestickImperativeHandle } from "../../ReactImgui/PlotCandlestick";
import { PlotCandlestickDataItem } from "../../ReactImgui/types";
import { ReactImgui } from "../../ReactImgui";

type Props = { symbol: string };

export const CryptoCandlestickPlot = ({ symbol }: Props) => {
    const dataService = useDataService();

    const plotRef = useRef<PlotCandlestickImperativeHandle>(null);

    // const [axisAutoFit, setAxisAutoFit] = useState(true);

    useEffect(() => {
        const subscription = dataService
            .getCryptoBarDatasets()
            .pipe(map((cryptoBars) => cryptoBars[symbol]))
            .subscribe((cryptoBarsForSymbol) => {
                if (plotRef.current) {
                    const data: PlotCandlestickDataItem[] = cryptoBarsForSymbol.map(
                        (cryptoBar) => ({
                            date: Number(new Date(cryptoBar.Timestamp)) / 1000,
                            open: cryptoBar.Open,
                            close: cryptoBar.Close,
                            high: cryptoBar.High,
                            low: cryptoBar.Low,
                        }),
                    );

                    // console.log(data);

                    plotRef.current.setData(data);
                }
            });

        return () => {
            subscription.unsubscribe();
        };
    }, [dataService]);

    // const toggleAxisAutoFit = useCallback(() => {
    //     setAxisAutoFit((val) => !val);
    // }, []);

    return (
        <ReactImgui.PlotCandlestick
            ref={plotRef}
            style={{ width: "100%", height: 400 }}
            // axisAutoFit={axisAutoFit}
        />
    );
};

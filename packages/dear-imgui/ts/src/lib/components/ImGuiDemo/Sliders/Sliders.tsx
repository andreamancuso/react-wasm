import React, { SyntheticEvent, useCallback, useMemo, useRef, useState } from "react";

export const Sliders = () => {
    const [singleSliderValue, setSingleSliderValue] = useState(0);
    const [doubleSliderValue, setDoubleSliderValue] = useState<[number, number]>([0, 0]);
    const [tripleSliderValue, setTripleSliderValue] = useState<[number, number, number]>([0, 0, 0]);
    const [quadSliderValue, setQuadSliderValue] = useState<[number, number, number, number]>([
        0, 0, 0, 0,
    ]);

    const handleTripleSliderValueChanged = useCallback((event: any) => {
        if (event.nativeEvent) {
            setTripleSliderValue([
                event.nativeEvent.values[0],
                event.nativeEvent.values[1],
                event.nativeEvent.values[2],
            ]);
        }
    }, []);

    const handleQuadSliderValueChanged = useCallback((event: any) => {
        if (event.nativeEvent) {
            setQuadSliderValue([
                event.nativeEvent.values[0],
                event.nativeEvent.values[1],
                event.nativeEvent.values[2],
                event.nativeEvent.values[3],
            ]);
        }
    }, []);
};

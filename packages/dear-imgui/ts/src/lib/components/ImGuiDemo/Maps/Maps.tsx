import React, { useCallback, useMemo, useRef, useState } from "react";
import { ReactImgui } from "src/lib/components/ReactImgui/components";
import { MapImperativeHandle } from "../../ReactImgui/Map";

export const Maps = () => {
    const mapRef = useRef<MapImperativeHandle>(null);
    const [center, setCenter] = useState<[number, number]>([0, 0]);
    const [zoom, setZoom] = useState(1);

    const renderMap = useCallback(() => {
        if (mapRef.current) {
            mapRef.current.render(center[0], center[1], zoom);
        }
    }, [center, zoom]);

    const locate = useCallback(() => {
        if ("geolocation" in navigator) {
            navigator.geolocation.getCurrentPosition((position) => {
                setCenter([position.coords.longitude, position.coords.latitude]);
            });
        }
    }, []);

    return (
        <ReactImgui.Fragment>
            <ReactImgui.SameLine>
                <ReactImgui.UnformattedText text={`Center: ${center[0]}, ${center[1]}`} />
                <ReactImgui.Button onClick={locate} label="Locate" />
                <ReactImgui.Button onClick={renderMap} label="Render" />
            </ReactImgui.SameLine>
            <ReactImgui.Map ref={mapRef} />
        </ReactImgui.Fragment>
    );
};

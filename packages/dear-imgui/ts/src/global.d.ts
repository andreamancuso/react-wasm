import { ReactElementWidgetsFlat } from "./lib/components/ReactImgui/types";

export {};

declare global {
    namespace JSX {
        interface IntrinsicElements {
            widget: ReactElementWidgetsFlat;
        }
    }

    declare var Jimp: typeof import("jimp");
}

declare module "*.osm" {
    const content: any;
    export default content;
}

declare module "*.pbf" {
    const content: any;
    export default content;
}

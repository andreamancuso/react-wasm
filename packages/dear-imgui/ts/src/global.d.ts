import { ReactElementWidgetsFlat } from "./lib/components/ReactImgui/types";

export {};

declare global {
    namespace JSX {
        interface IntrinsicElements {
            widget: ReactElementWidgetsFlat;
        }
    }
}

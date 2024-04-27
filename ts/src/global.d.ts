import { ReactElementWidgetsFlat } from "./components/ReactImgui/types";

export {};

declare global {
    namespace JSX {
        interface IntrinsicElements {
            widget: ReactElementWidgetsFlat;
        }
    }
}

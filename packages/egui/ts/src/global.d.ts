import { ReactElementWidgetsFlat } from "./lib/components/ReactEgui/types";

export {};

declare global {
    namespace JSX {
        interface IntrinsicElements {
            widget: ReactElementWidgetsFlat;
        }
    }
}

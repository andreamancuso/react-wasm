import { create } from "zustand";
import { cryptoSymbols } from "./cryptoSymbols";

export type State = {
    symbols: string[];
    cryptoAssets: any[];
    cryptoQuotes: any;
    setCryptoAssets: (cryptoAssets: any[]) => void;
    setCryptoQuote: (cryptoQuote: any) => void;
};

export const useStore = create<State>((set) => ({
    cryptoAssets: [],
    symbols: cryptoSymbols,
    cryptoQuotes: {},
    setCryptoAssets: (cryptoAssets) => set(() => ({ cryptoAssets })),
    setCryptoQuote: (cryptoQuote) =>
        set((state) => ({
            ...state,
            cryptoQuotes: { ...state.cryptoQuotes, [cryptoQuote.S]: cryptoQuote },
        })),
}));

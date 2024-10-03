import { ReplaySubject } from "rxjs";
import { CryptoQuote } from "@alpacahq/alpaca-trade-api/dist/resources/datav2/entityv2";

export type CryptoQuoteWithSymbol = CryptoQuote & {
    S: string;
};

export class DataService {
    private cryptoQuotes: ReplaySubject<CryptoQuoteWithSymbol>;

    constructor() {
        this.cryptoQuotes = new ReplaySubject(100000);
    }

    addCryptoQuote(cryptoQuote: CryptoQuoteWithSymbol) {
        this.cryptoQuotes.next(cryptoQuote);
    }

    getCryptoQuotes() {
        return this.cryptoQuotes.asObservable();
    }
}

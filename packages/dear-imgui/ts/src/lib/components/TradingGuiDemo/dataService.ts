import { ReplaySubject } from "rxjs";
import {
    CryptoQuote,
    CryptoSnapshot,
} from "@alpacahq/alpaca-trade-api/dist/resources/datav2/entityv2";

export type CryptoQuoteWithSymbol = CryptoQuote & {
    S: string;
};

export type CryptoSnapshots = {
    [key: string]: CryptoSnapshot;
};

export class DataService {
    private cryptoQuotes: ReplaySubject<CryptoQuoteWithSymbol>;
    private cryptoSnapshots: ReplaySubject<CryptoSnapshots>;

    constructor() {
        this.cryptoQuotes = new ReplaySubject(100000);
        this.cryptoSnapshots = new ReplaySubject(100000);
    }

    addCryptoQuote(cryptoQuote: CryptoQuoteWithSymbol) {
        this.cryptoQuotes.next(cryptoQuote);
    }

    addCryptoSnapshot(cryptoSnapshots: CryptoSnapshots) {
        this.cryptoSnapshots.next(cryptoSnapshots);
    }

    getCryptoQuotes() {
        return this.cryptoQuotes.asObservable();
    }

    getCryptoSnapshots() {
        return this.cryptoSnapshots.asObservable();
    }
}

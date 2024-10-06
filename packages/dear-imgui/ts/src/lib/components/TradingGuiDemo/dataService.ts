import { ReplaySubject } from "rxjs";
import {
    CryptoBar,
    CryptoQuote,
    CryptoSnapshot,
} from "@alpacahq/alpaca-trade-api/dist/resources/datav2/entityv2";

export type CryptoQuoteWithSymbol = CryptoQuote & {
    S: string;
};

export type CryptoSnapshots = {
    [key: string]: CryptoSnapshot;
};

export type CryptoBars = {
    [key: string]: CryptoBar;
};

export class DataService {
    private cryptoQuotes: ReplaySubject<CryptoQuoteWithSymbol>;
    private cryptoSnapshots: ReplaySubject<CryptoSnapshots>;
    private cryptoBars: ReplaySubject<CryptoBars>;

    constructor() {
        this.cryptoQuotes = new ReplaySubject(100000);
        this.cryptoSnapshots = new ReplaySubject(100000);
        this.cryptoBars = new ReplaySubject(100000);
    }

    addCryptoQuote(cryptoQuote: CryptoQuoteWithSymbol) {
        this.cryptoQuotes.next(cryptoQuote);
    }

    addCryptoSnapshots(cryptoSnapshots: CryptoSnapshots) {
        this.cryptoSnapshots.next(cryptoSnapshots);
    }

    addCryptoBars(cryptoBars: CryptoBars) {
        this.cryptoBars.next(cryptoBars);
    }

    getCryptoQuotes() {
        return this.cryptoQuotes.asObservable();
    }

    getCryptoSnapshots() {
        return this.cryptoSnapshots.asObservable();
    }

    getCryptoBars() {
        return this.cryptoBars.asObservable();
    }
}

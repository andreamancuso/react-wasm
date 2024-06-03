export type StyleRules = {
    font?: {
        name: string;
        size: number;
    };
    color?: string;
};

export type RWStyleSheet = {
    [k: string]: StyleRules;
};

const RWStyleSheet = {
    create: (styleSheet: RWStyleSheet): RWStyleSheet => styleSheet,
};

export default RWStyleSheet;

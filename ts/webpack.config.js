const { CleanWebpackPlugin } = require("clean-webpack-plugin");
const HtmlWebpackPlugin = require("html-webpack-plugin");
const MiniCssExtractPlugin = require("mini-css-extract-plugin");
const webpack = require("webpack");
const path = require("path");

module.exports = [
    {
        name: "main",
        entry: path.resolve(__dirname, "./src/index.tsx"),
        mode: "development",
        devServer: {
            port: 3000,
            open: true,
            headers: {
                "Access-Control-Allow-Origin": "*",
                "Cross-Origin-Embedder-Policy": "require-corp",
                "Cross-Origin-Opener-Policy": "same-origin",
            },
        },
        experiments: {
            asyncWebAssembly: true,
            syncWebAssembly: true,
        },
        resolve: {
            extensions: [".ts", ".tsx", ".js", ".mjs", ".css"],
            modules: [path.resolve(__dirname, "./"), path.resolve(__dirname, "./node_modules/")],
            alias: {
                "@react-native": path.resolve(__dirname, "react-native/"),
                "react-native": path.resolve(__dirname, "react-native/"),
            },
        },
        module: {
            rules: [
                {
                    test: /\.(m?js|jsx|tsx|ts)$/,
                    include: [path.resolve(__dirname, "src")],
                    exclude: /node_modules/,
                    use: {
                        loader: "babel-loader",
                        options: {
                            presets: [
                                ["@babel/preset-env", { targets: "defaults" }],
                                "@babel/preset-typescript",
                                ["@babel/preset-react", { runtime: "automatic" }],
                            ],
                            plugins: ["@babel/plugin-proposal-class-properties"],
                        },
                    },
                },
                {
                    test: /\.css$/i,
                    use: [MiniCssExtractPlugin.loader, "css-loader"],
                },
                {
                    test: /\.(ico|icns|eot|woff|woff2|jpe?g|png)$/,
                    // exclude: [path.resolve(__dirname, "react-native")],
                    use: [
                        {
                            // loader: "asset/inline",
                            loader: "url-loader",
                        },
                    ],
                },
            ],
        },
        plugins: [
            new MiniCssExtractPlugin({ ignoreOrder: true }),
            new HtmlWebpackPlugin({
                template: path.resolve(__dirname, "./public/index.html"),
            }),
            new CleanWebpackPlugin(),
            new webpack.HotModuleReplacementPlugin(),
        ],
    },
    {
        name: "fabric-worker",
        target: "webworker",
        mode: "development",
        entry: path.resolve(__dirname, "./src/fabric-worker.ts"),
        output: {
            path: path.resolve(__dirname, "dist"),
            filename: "fabric-worker.js",
        },
        resolve: {
            extensions: [".ts", ".tsx", ".js", ".jsx", ".mjs"],
            modules: [path.resolve(__dirname, "./"), path.resolve(__dirname, "./node_modules/")],
        },
        module: {
            rules: [
                {
                    test: /\.(m?js|jsx|tsx|ts)$/,
                    include: [path.resolve(__dirname, "src")],
                    exclude: /node_modules/,
                    use: {
                        loader: "babel-loader",
                        options: {
                            presets: [
                                ["@babel/preset-env", { targets: "defaults" }],
                                "@babel/preset-typescript",
                                ["@babel/preset-react", { runtime: "automatic" }],
                            ],
                            plugins: ["@babel/plugin-proposal-class-properties"],
                        },
                    },
                },
                {
                    test: /\.(ico|icns|eot|woff|woff2|jpe?g|png)$/,
                    // exclude: [path.resolve(__dirname, "react-native")],
                    use: [
                        {
                            // loader: "asset/inline",
                            loader: "url-loader",
                        },
                    ],
                },
            ],
        },
    },
];

const { CleanWebpackPlugin } = require("clean-webpack-plugin");
const HtmlWebpackPlugin = require("html-webpack-plugin");
const MiniCssExtractPlugin = require("mini-css-extract-plugin");
const webpack = require("webpack");
const path = require("path");

module.exports = {
    entry: path.resolve(__dirname, "./src/index.tsx"),
    mode: "development",
    devServer: {
        port: 3000,
        open: true,
        headers: {
            "Access-Control-Allow-Origin": "*",
        },
    },
    experiments: {
        asyncWebAssembly: true,
        syncWebAssembly: true,
    },
    resolve: {
        extensions: [".ts", ".tsx", ".js", ".mjs", ".css"],
        modules: [path.resolve(__dirname, "./"), path.resolve(__dirname, "./node_modules/")],
    },
    module: {
        rules: [
            {
                test: /\.(m?js|jsx|tsx|ts)$/,
                exclude: /node_modules/,
                use: ["babel-loader"],
            },
            {
                test: /\.css$/i,
                use: [MiniCssExtractPlugin.loader, "css-loader"],
            },
            {
                test: /\.(ico|icns|eot|woff|woff2|jpe?g)$/,
                use: [
                    {
                        loader: "file-loader",
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
};

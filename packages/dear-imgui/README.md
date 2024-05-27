# React bindings for Dear ImGui (and ImPlot)

Please note that only Chrome and Edge are currently supported. Firefox Nightlies should also work but no testing has been carried out on them.

## Building instructions

The repo ships with a pre-generated WASM file and related .mjs file. You may want to make changes and build new WASM files. Please follow these instructions. Feel free to create an issue if you experience any difficulties. If you are a Linux or Mac user and manage to get it working on your local environment, please feel free to share your findings.

### Requirements

Tested with:

-   Windows 11
-   Visual Studio 2022
-   emscripten 3.1.60, follow these instructions https://emscripten.org/docs/getting_started/downloads.html
-   Node.js v20 (LTS) download it from https://nodejs.org/dist/v20.13.1/node-v20.13.1-x64.msi or install via [Node Version Manager](https://github.com/coreybutler/nvm-windows)
-   Yarn, follow these instructions https://yarnpkg.com/getting-started/install

### Setup

#### Emscripten

-   Open a VS Studio 2022 developer prompt
-   Run `git submodule update --init --recursive` to retrieve all dependencies in the `deps` folder
-   Install emsdk:
    -   `git clone https://github.com/emscripten-core/emsdk.git`
    -   `cd emsdk`
    -   `./emsdk install 3.1.60`
    -   `./emsdk activate 3.1.60`
    -   cd into `emsdk/upstream/emscripten` and run `npm install` (this ensures `tsc` is available so that TypeScript typings can be generated)

#### React and other dependencies

-   `cd react-wasm/packages/dear-imgui/ts`
-   `yarn add -P react@18.2.0 react-dom@18.2.0`

## Building

-   Open a Windows CLI (not PowerShell)
-   **IMPORTANT**: Run `<emsdk-dir>/emsdk_env.bat` (if you haven't done so already)
-   `cd react-wasm/packages/dear-imgui/`
-   Build the WASM, either via Yarn:
    -   `cd ts`
    -   `yarn`
    -   `yarn emcmake`
    -   `yarn cmake`
-   or directly via CMake:
    -   `cd cpp`
    -   `cmake -S . -B build -GNinja`
    -   `cmake --build ./build --target reactDearImgui`

## Verifying

-   `cd react-wasm/packages/dear-imgui/ts`
-   `yarn start`

Your default browser will open, the URL should http://localhost:3000

Please note, not all browsers are currently supported.

## Notes

Terminating the WASM process throws an `ExitStatus` exception - this is expected, so long as the status is `0` then it's all good.

`Pthread` support was recently added. At the time of writing this causes Webpack to issue a warning on startup. The warning can be dismissed by clicking on the close (X) button. Enabling the `-s STRICT` linker flag should fix it, however this currently introduces our issues.

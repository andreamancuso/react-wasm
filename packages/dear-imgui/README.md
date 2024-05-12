# React bindings for Dear ImGui (and ImPlot)

Please note: only latest Chrome and Edge are currently supported.

## Building instructions

The repo ships with a pre-generated WASM file and related .mjs file. You may want to make changes and build new WASM files. Please follow these instructions. Feel free to create an issue if you experience any difficulties. If you are a Linux or Mac user and manage to get it working on your local environment, please feel free to share your findings.

### Requirements

Tested with:

-   Windows 11
-   emscripten 3.1.54, follow these instructions https://emscripten.org/docs/getting_started/downloads.html
-   CMake 3.29.1, download it from https://github.com/Kitware/CMake/releases
-   ninja 1.12.0, download it from https://github.com/ninja-build/ninja/releases

### Setup

-   Run `git submodule update --init --recursive` to retrieve all dependencies in the `deps` folder
-   Install emsdk:
    - `git clone https://github.com/emscripten-core/emsdk.git`
    - `cd emsdk`
    - `./emsdk install 3.1.54`
    - `./emsdk activate 3.1.54`
-   Install Ninja and add it to `%PATH%`


## Building

-   Open a Windows CLI (not PowerShell)
-   Run `<emsdk-dir>/emsdk_env.bat` (if you haven't done so already)
-   `cd react-imgui/src/cpp`
-   Build the WASM:
    -   `cd ts`
    -   `yarn`
    -   `yarn emcmake`
    -   `yarn cmake`

## Notes

Terminating the WASM process throws an `ExitStatus` exception - this is expected, so long as the status is `0` then it's all good.

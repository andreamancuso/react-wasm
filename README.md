# WASM views

## Requirements

Tested with:

-   Visual Studio 2022 Community
-   emscripten 3.1.54, follow these instructions https://emscripten.org/docs/getting_started/downloads.html
-   CMake 3.29.1, download it from https://github.com/Kitware/CMake/releases
-   ninja 1.12.0, download it from https://github.com/ninja-build/ninja/releases

## Setup

-   Run `git submodule update --init --recursive` to retrieve all dependencies in the `deps` folder
-   Install emsdk:
    - `git clone https://github.com/emscripten-core/emsdk.git`
    - `cd emsdk`
    - `./emsdk install 3.1.54`
    - `./emsdk activate 3.1.54`
-   Install CMake and add `bin` to `%PATH%`
-   Install Ninja and add it to `%PATH%`
-   Prepare CMake:
    - `cd react-imgui/src/cpp`
    - `emcmake cmake .`

## Building

-   Open a Windows CLI (not PowerShell)
-   Run `<emsdk-dir>/emsdk_env.bat` (if you haven't done so already)
-   `cd react-imgui/src/cpp`
-   If this is the first time you're attempting to build a WASM file, run `npm run cmake`

## Notes

Terminating the WASM process throws an `ExitStatus` exception - this is expected, so long as the status is `0` then it's all good.

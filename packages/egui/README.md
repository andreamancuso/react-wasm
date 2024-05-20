# React bindings for egui

WIP

### Requirements

Tested with:

-   Ubuntu Linux 22.04 LTS (native)
-   WSL 2 (Ubuntu Linux 22.04 LTS)
-   Rust 1.76
-   Rust Rover (EAP)
-   wasm-pack

### Setup

-   Install Rust by running `curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh` (this should work on both Ubuntu and WSL 2)
-   Run `sudo apt-get install libxcb-render0-dev libxcb-shape0-dev libxcb-xfixes0-dev libxkbcommon-dev libssl-dev`
-   `rustup target add wasm32-unknown-unknown`

Rust Rover settings:

-   add a WSL (Ubuntu 22.04) target; you will need to specify paths to both `rustc` and `cargo` (typically found in `/home/<user>/.cargo/bin/rustc` and `/home/<user>/.cargo/bin/cargo`); **IMPORTANT**: `Project default target` should be `Local machine` otherwise Rust Rover will pass a `--distribution` cli parameter to cargo, resulting in an error
-   Ensure the correct toolchain is selected (v1.76)

Finally, create a `wasm-pack` Run configuration ensuring the command is `build --target web`

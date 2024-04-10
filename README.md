# Micro RPG
A tiny RPG I'm making using C and Raylib.

## Requirements
You must the `clang` compiler installed to build for a native target.
You must have the `emscripten` compiler installed to build for WASM.

## Usage
To build for OSX, use `make`.

To build for WASM using the default emscripten-generated HTML:
* In a terminal, run `make build_wasm`.
* In a terminal, run `emrun bin/wasm_game.html`. This causes emscripten to spawn a server, open a browser window, and serve the file from that server. This avoicd CORS errors.

To buil for WASM using using a stripped-down HTML without all the emscripten branding and console output:
* In a terminal, run `make build_wasm`.
* Start an HTTP server in the root directory, then request `game.html` from the server. If you try to open the HTML file directly, you'll get CORS errors.
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
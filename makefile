OSX_COMPILER = clang
OSX_SOURCE_LIBS = -Inative-lib/ -Isrc/
OSX_OPT = -Lnative-lib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL native-lib/libraylib.a
OSX_OUT = -o "bin/osx_game"

WASM_COMPILER = emcc
WASM_SOURCE_LIBS = -Iwasm-lib/ -Isrc/
WASM_ASSET_DIR = --preload-file assets@/assets
WASM_OPT = -Lwasm-lib/ -lraylib wasm-lib/libraylib.a -DPLATFORM_WEB=1 -s USE_GLFW=3 -s WASM=1 -s USE_WEBGL2=1 --emrun
WASM_OUT = -o "bin/wasm_game.html"

CFILES = src/ui/*.c src/asset/*.c src/common/*.c src/battle/*.c

build_osx:
	$(OSX_COMPILER) src/main.c $(CFILES) $(OSX_SOURCE_LIBS) $(OSX_OUT) $(OSX_OPT)

build_wasm:
	$(WASM_COMPILER) src/main.c $(CFILES) $(WASM_SOURCE_LIBS) $(WASM_OUT) $(WASM_OPT) ${WASM_ASSET_DIR}
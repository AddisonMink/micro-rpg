OSX_COMPILER = clang
OSX_ARM_SOURCE_LIBS = -Iosx-arm-lib/ -Isrc/
OSX_X86_SOURCE_LIBS = -Iosx-x86-lib/ -Isrc/
OSX_ARM_OPT = -Losx-arm-lib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL osx-arm-lib/libraylib.a
OSX_X86_OPT = -Losx-x86-lib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL osx-x86-lib/libraylib.a
OSX_OUT = -o "bin/osx_game"

OSX_X86_SOURCE_LIBS = -Iosx-x86-lib/ -Isrc/

WASM_COMPILER = emcc
WASM_SOURCE_LIBS = -Iwasm-lib/ -Isrc/
WASM_ASSET_DIR = --preload-file assets@/assets
WASM_OPT = -Lwasm-lib/ -lraylib wasm-lib/libraylib.a -DPLATFORM_WEB=1 -s USE_GLFW=3 -s WASM=1 -s USE_WEBGL2=1 --emrun
WASM_OUT = -o "bin/wasm_game.html"

CFILES = src/ui/*.c src/asset/*.c src/common/*.c src/battle/*.c

build_osx_x86:
	$(OSX_COMPILER) src/main.c $(CFILES) $(OSX_X86_SOURCE_LIBS) $(OSX_OUT) $(OSX_X86_OPT)

build_osx_arm:
	$(OSX_COMPILER) src/main.c $(CFILES) $(OSX_ARM_SOURCE_LIBS) $(OSX_OUT) $(OSX_ARM_OPT)

build_wasm:
	$(WASM_COMPILER) src/main.c $(CFILES) $(WASM_SOURCE_LIBS) $(WASM_OUT) $(WASM_OPT) $(WASM_ASSET_DIR)
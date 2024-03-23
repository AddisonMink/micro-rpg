COMPILER = clang
SOURCE_LIBS = -Ilib/ -Isrc/
OSX_OPT = -Llib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib.a
OSX_OUT = -o "bin/build_osx"
CFILES = $(wildcard src/asset/*.c) $(wildcard src/ui/*.c) $(wildcard src/battle/*.c) $(wildcard src/battle/draw/*.c) $(wildcard src/battle/update/*.c)

build_osx:
	$(COMPILER) src/main.c $(CFILES) $(SOURCE_LIBS) $(OSX_OUT) $(OSX_OPT)

test_enemy_ui_osx:
	$(COMPILER) test/battle/draw/test-enemy-ui.c $(CFILES) $(SOURCE_LIBS) $(OSX_TEST_OUT) -o "bin/test_enemy_ui_osx" $(OSX_OPT)
COMPILER = clang
SOURCE_LIBS = -Ilib/
OSX_OPT = -Llib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib.a
OSX_OUT = -o "bin/build_osx"
CFILES = $(wildcard src/*.c) $(wildcard src/ui/*.c) $(wildcard src/battle/*.c) $(wildcard src/battle/draw/*.c) $(wildcard src/battle/update/*.c)

build_osx:
	$(COMPILER) $(CFILES) $(SOURCE_LIBS) $(OSX_OUT) $(OSX_OPT)
#!/bin/bash

options="-O2 -Wall -mwindows"
includes="-Ilibs/tail/include -Isrc/headers"

i686-w64-mingw32-gcc \
-DBURDOCK_SRC="$( libs/tail/tools/obj-parser.exe assets/mesh/burdock.obj smooth )" \
${includes} \
${options} \
-o game.exe \
src/*.c \
libs/tail/static/tail.a
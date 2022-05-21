#!/bin/bash

options="-O2 -Wall -mwindows"
includes="-Ilibs/tail/include -Isrc/headers"

i686-w64-mingw32-gcc \
${includes} \
${options} \
-o game.exe \
src/*.c \
libs/tail/tail.a
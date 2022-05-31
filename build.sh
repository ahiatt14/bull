#!/bin/bash

tools="libs/tail/tools/"
options="-O2 -Wall -mwindows"
includes="-Ilibs/tail/include -Isrc/headers"

rm -rf game.exe
./${tools}obj-parser.exe assets/mesh/burdock.obj smooth &> src/headers/burdock_mesh.h && \
./${tools}obj-parser.exe assets/mesh/capsule_apartment.obj smooth &> src/headers/capsule_apartment_mesh.h && \
./${tools}validate-glsl.exe assets/glsl/default_vert.glsl vert && \
./${tools}validate-glsl.exe assets/glsl/solid_color_frag.glsl frag && \
./${tools}validate-glsl.exe assets/glsl/normal_debug_frag.glsl frag && \
./${tools}headerfy-glsl.exe assets/glsl/solid_color_frag.glsl &> src/headers/solid_color_frag.h && \
./${tools}headerfy-glsl.exe assets/glsl/normal_debug_frag.glsl &> src/headers/normal_debug_frag.h && \
./${tools}headerfy-glsl.exe assets/glsl/default_vert.glsl &> src/headers/default_vert.h && \
i686-w64-mingw32-gcc \
${includes} \
${options} \
-o game.exe \
src/*.c \
libs/tail/static/tail.a
rm -f src/headers/burdock_mesh.h
rm -f src/headers/capsule_apartment_mesh.h
rm -f src/headers/solid_color_frag.h
rm -f src/headers/normal_debug_frag.h
rm -f src/headers/default_vert.h
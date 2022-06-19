#!/bin/bash

tools="libs/tail/tools/"
options="-O2 -Wall"
# -mwindows
includes="-Ilibs/tail/include -Isrc/headers"

# TODO: this ain't sustainable!

rm -rf game.exe
./${tools}obj-parser.exe assets/mesh/burdock.obj smooth src/headers/ src/ && \
./${tools}obj-parser.exe assets/mesh/capsule_apartment.obj smooth src/headers/ src/ && \
./${tools}validate-glsl.exe assets/glsl/default_vert.glsl vert && \
./${tools}validate-glsl.exe assets/glsl/solid_color_frag.glsl frag && \
./${tools}validate-glsl.exe assets/glsl/leaf_frag.glsl frag && \
./${tools}validate-glsl.exe assets/glsl/normal_debug_frag.glsl frag && \
./${tools}validate-glsl.exe assets/glsl/sky_frag.glsl frag && \
./${tools}sourcify-glsl.exe assets/glsl/sky_frag.glsl src/headers/ src/ && \
./${tools}sourcify-glsl.exe assets/glsl/leaf_frag.glsl src/headers/ src/ && \
./${tools}sourcify-glsl.exe assets/glsl/solid_color_frag.glsl src/headers/ src/ && \
./${tools}sourcify-glsl.exe assets/glsl/normal_debug_frag.glsl src/headers/ src/ && \
./${tools}sourcify-glsl.exe assets/glsl/default_vert.glsl src/headers/ src/ && \
i686-w64-mingw32-gcc \
${includes} \
${options} \
-o game.exe \
src/*.c \
libs/tail/static/tail.a

rm -f src/headers/burdock_mesh.h
rm -f src/headers/capsule_apartment_mesh.h

rm -f src/burdock_mesh.c
rm -f src/capsule_apartment_mesh.c

rm -f src/headers/solid_color_frag.h
rm -f src/headers/normal_debug_frag.h
rm -f src/headers/default_vert.h
rm -f src/headers/sky_frag.h
rm -f src/headers/leaf_frag.h

rm -f src/solid_color_frag.c
rm -f src/normal_debug_frag.c
rm -f src/default_vert.c
rm -f src/sky_frag.c
rm -f src/leaf_frag.c
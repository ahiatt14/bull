#!/bin/bash

tools="libs/tail/tools/"
# -mwindows
options="-O2 -Wall"
includes="-Ilibs/tail/include -Isrc/headers -Isrc/player -Isrc/scenes"

# TODO: this ain't sustainable!

rm -rf game.exe

./${tools}validate-glsl.exe assets/glsl/default_vert.glsl vert && \
./${tools}validate-glsl.exe assets/glsl/solid_color_frag.glsl frag && \
./${tools}validate-glsl.exe assets/glsl/leaf_frag.glsl frag && \
./${tools}validate-glsl.exe assets/glsl/normal_debug_frag.glsl frag && \
./${tools}validate-glsl.exe assets/glsl/flat_texture_frag.glsl frag && \
./${tools}sourcify-glsl.exe assets/glsl/flat_texture_frag.glsl src/headers/ src/ && \
./${tools}sourcify-glsl.exe assets/glsl/leaf_frag.glsl src/headers/ src/ && \
./${tools}sourcify-glsl.exe assets/glsl/solid_color_frag.glsl src/headers/ src/ && \
./${tools}sourcify-glsl.exe assets/glsl/normal_debug_frag.glsl src/headers/ src/ && \
./${tools}sourcify-glsl.exe assets/glsl/default_vert.glsl src/headers/ src/ && \
./${tools}sourcify-png.exe assets/png/clod256.png 3 src/headers/clod256_texture.h src/clod256_texture.c && \
./${tools}sourcify-png.exe assets/png/concrete32bit.png 3 src/headers/concrete32bit_texture.h src/concrete32bit_texture.c && \
./${tools}sourcify-obj.exe assets/mesh/burdock.obj smooth src/headers/ src/ && \
./${tools}sourcify-obj.exe assets/mesh/pyramid.obj flat src/headers/ src/ && \
./${tools}sourcify-obj.exe assets/mesh/sphere.obj flat src/headers/ src/ && \
./${tools}sourcify-obj.exe assets/mesh/capsule_apartment.obj smooth src/headers/ src/ && \
i686-w64-mingw32-gcc \
${includes} \
${options} \
-o game.exe \
src/*.c \
src/scenes/*.c \
src/player/*.c \
src/static_assets/*.c \
libs/tail/static/tail.a

rm -f src/headers/burdock_mesh.h
rm -f src/headers/pyramid_mesh.h
rm -f src/headers/capsule_apartment_mesh.h
rm -f src/headers/sphere_mesh.h

rm -f src/burdock_mesh.c
rm -f src/capsule_apartment_mesh.c
rm -f src/pyramid_mesh.c
rm -f src/sphere_mesh.c

rm -f src/headers/clod256_texture.h
rm -f src/clod256_texture.c

rm -f src/headers/concrete32bit_texture.h
rm -f src/concrete32bit_texture.c

rm -f src/headers/solid_color_frag.h
rm -f src/headers/normal_debug_frag.h
rm -f src/headers/default_vert.h
rm -f src/headers/flat_texture_frag.h
rm -f src/headers/leaf_frag.h

rm -f src/solid_color_frag.c
rm -f src/normal_debug_frag.c
rm -f src/default_vert.c
rm -f src/flat_texture_frag.c
rm -f src/leaf_frag.c
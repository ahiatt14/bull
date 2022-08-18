#!/bin/bash

tools="libs/tail/tools/"
options="-O2 -Wall"
includes="-Ilibs/tail/include -Isrc/headers \
-Isrc/player -Isrc/scenes -Isrc/gameplay -Isrc/background"

# TODO: this ain't sustainable!

rm -rf game.exe

./${tools}validate-glsl.exe assets/glsl/default_vert.glsl vert && \
./${tools}validate-glsl.exe assets/glsl/solid_color_frag.glsl frag && \
./${tools}validate-glsl.exe assets/glsl/flat_texture_frag.glsl frag && \
./${tools}validate-glsl.exe assets/glsl/water_surface_frag.glsl frag \
./${tools}validate-glsl.exe assets/glsl/core_frag.glsl frag && \
./${tools}validate-glsl.exe assets/glsl/turbine_frag.glsl frag && \
./${tools}validate-glsl.exe assets/glsl/sky_frag.glsl frag && \
./${tools}validate-glsl.exe assets/glsl/bouncer_frag.glsl frag && \
./${tools}validate-glsl.exe assets/glsl/steam_frag.glsl frag && \
./${tools}validate-glsl.exe assets/glsl/normal_debug_frag.glsl frag && \
./${tools}validate-glsl.exe assets/glsl/normal_debug_vert.glsl vert && \
./${tools}validate-glsl.exe assets/glsl/normal_debug_geo.glsl geo \
&& \
./${tools}sourcify-glsl.exe assets/glsl/flat_texture_frag.glsl src/headers/ src/ && \
./${tools}sourcify-glsl.exe assets/glsl/solid_color_frag.glsl src/headers/ src/ && \
./${tools}sourcify-glsl.exe assets/glsl/default_vert.glsl src/headers/ src/ && \
./${tools}sourcify-glsl.exe assets/glsl/water_surface_frag.glsl src/headers/ src/ && \
./${tools}sourcify-glsl.exe assets/glsl/core_frag.glsl src/headers/ src/ && \
./${tools}sourcify-glsl.exe assets/glsl/turbine_frag.glsl src/headers/ src/ && \
./${tools}sourcify-glsl.exe assets/glsl/sky_frag.glsl src/headers/ src/ && \
./${tools}sourcify-glsl.exe assets/glsl/bouncer_frag.glsl src/headers/ src/ && \
./${tools}sourcify-glsl.exe assets/glsl/steam_frag.glsl src/headers/ src/ && \
./${tools}sourcify-glsl.exe assets/glsl/normal_debug_frag.glsl src/headers/ src/ && \
./${tools}sourcify-glsl.exe assets/glsl/normal_debug_vert.glsl src/headers/ src/ && \
./${tools}sourcify-glsl.exe assets/glsl/normal_debug_geo.glsl src/headers/ src/ \
&& \
./${tools}sourcify-png.exe assets/png/clouds.png 3 src/headers/clouds_texture.h src/clouds_texture.c && \
./${tools}sourcify-png.exe assets/png/water.png 3 src/headers/water_texture.h src/water_texture.c && \
./${tools}sourcify-png.exe assets/png/cloud_cover.png 3 src/headers/cloud_cover_texture.h src/cloud_cover_texture.c \
&& \
./${tools}sourcify-obj.exe assets/mesh/burdock.obj smooth src/headers/ src/ && \
./${tools}sourcify-obj.exe assets/mesh/pyramid.obj flat src/headers/ src/ && \
./${tools}sourcify-obj.exe assets/mesh/exclamation.obj flat src/headers/ src/ && \
./${tools}sourcify-obj.exe assets/mesh/capsule_apartment.obj smooth src/headers/ src/ && \
./${tools}sourcify-obj.exe assets/mesh/core.obj smooth src/headers/ src/ && \
./${tools}sourcify-obj.exe assets/mesh/cage.obj smooth src/headers/ src/ && \
./${tools}sourcify-obj.exe assets/mesh/sphere.obj smooth src/headers/ src/ && \
./${tools}sourcify-obj.exe assets/mesh/bird.obj flat src/headers/ src/ && \
./${tools}sourcify-obj.exe assets/mesh/turbine_base.obj flat src/headers/ src/ && \
./${tools}sourcify-obj.exe assets/mesh/turbine_blades.obj flat src/headers/ src/ && \
./${tools}sourcify-obj.exe assets/mesh/smooth_cube.obj flat src/headers/ src/ \
&& \
i686-w64-mingw32-gcc \
${includes} \
${options} \
-o game.exe \
src/*.c \
src/scenes/*.c \
src/player/*.c \
src/gameplay/*.c \
src/background/*.c \
libs/tail/static/tail.a

rm -f src/headers/core_mesh.h
rm -f src/headers/burdock_mesh.h
rm -f src/headers/pyramid_mesh.h
rm -f src/headers/capsule_apartment_mesh.h
rm -f src/headers/sphere_mesh.h
rm -f src/headers/exclamation_mesh.h
rm -f src/headers/cage_mesh.h
rm -f src/headers/bird_mesh.h
rm -f src/headers/turbine_blades_mesh.h
rm -f src/headers/turbine_base_mesh.h
rm -f src/headers/smooth_cube_mesh.h

rm -f src/core_mesh.c
rm -f src/burdock_mesh.c
rm -f src/capsule_apartment_mesh.c
rm -f src/pyramid_mesh.c
rm -f src/sphere_mesh.c
rm -f src/exclamation_mesh.c
rm -f src/cage_mesh.c
rm -f src/bird_mesh.c
rm -f src/turbine_blades_mesh.c
rm -f src/turbine_base_mesh.c
rm -f src/smooth_cube_mesh.c

rm -f src/headers/water_texture.h
rm -f src/water_texture.c
rm -f src/clouds_texture.c
rm -f src/headers/clouds_texture.h
rm -f src/headers/cloud_cover_texture.h
rm -f src/cloud_cover_texture.c

rm -f src/headers/solid_color_frag.h
rm -f src/headers/normal_debug_frag.h
rm -f src/headers/default_vert.h
rm -f src/headers/flat_texture_frag.h
rm -f src/headers/leaf_frag.h
rm -f src/headers/core_frag.h
rm -f src/headers/water_surface_frag.h
rm -f src/headers/turbine_frag.h
rm -f src/headers/sky_frag.h
rm -f src/headers/bouncer_frag.h
rm -f src/headers/steam_frag.h
rm -f src/headers/normal_debug_geo.h
rm -f src/headers/normal_debug_vert.h

rm -f src/solid_color_frag.c
rm -f src/normal_debug_frag.c
rm -f src/default_vert.c
rm -f src/flat_texture_frag.c
rm -f src/leaf_frag.c
rm -f src/core_frag.c
rm -f src/water_surface_frag.c
rm -f src/turbine_frag.c
rm -f src/sky_frag.c
rm -f src/bouncer_frag.c
rm -f src/steam_frag.c
rm -f src/normal_debug_geo.c
rm -f src/normal_debug_vert.c
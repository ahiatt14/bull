#!/bin/bash

tools="libs/tail/tools/"
options="-O2 -Wall"
artifact_dir="src/temp_asset_src_files/"
includes="-Ilibs/tail/include -Isrc/headers \
-Isrc/player -Isrc/scenes -Isrc/gameplay -Isrc/background -I${artifact_dir}"

clean() {
  rm -f game.exe
  rm -rf obj
  rm -rf ${artifact_dir}
}

build_src() {

  rm -f game.exe

  i686-w64-mingw32-gcc \
  ${includes} \
  ${options} \
  -o game.exe \
  src/*.c \
  ${artifact_dir}*.c \
  src/scenes/*.c \
  src/player/*.c \
  src/gameplay/*.c \
  src/background/*.c \
  libs/tail/static/tail.a
}

build_assets() {

  rm -rf ${artifact_dir}
  mkdir ${artifact_dir}
  # rm -rf obj
  # mkdir obj

  # TODO: this ain't sustainable!
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
  ./${tools}validate-glsl.exe assets/glsl/normal_debug_geo.glsl geo && \
  ./${tools}validate-glsl.exe assets/glsl/alpha_texture_frag.glsl frag && \
  ./${tools}validate-glsl.exe assets/glsl/mountain_frag.glsl frag \
  && \
  ./${tools}sourcify-glsl.exe assets/glsl/flat_texture_frag.glsl ${artifact_dir} && \
  ./${tools}sourcify-glsl.exe assets/glsl/solid_color_frag.glsl ${artifact_dir} && \
  ./${tools}sourcify-glsl.exe assets/glsl/default_vert.glsl ${artifact_dir} && \
  ./${tools}sourcify-glsl.exe assets/glsl/water_surface_frag.glsl ${artifact_dir} && \
  ./${tools}sourcify-glsl.exe assets/glsl/core_frag.glsl ${artifact_dir} && \
  ./${tools}sourcify-glsl.exe assets/glsl/turbine_frag.glsl ${artifact_dir} && \
  ./${tools}sourcify-glsl.exe assets/glsl/sky_frag.glsl ${artifact_dir} && \
  ./${tools}sourcify-glsl.exe assets/glsl/bouncer_frag.glsl ${artifact_dir} && \
  ./${tools}sourcify-glsl.exe assets/glsl/steam_frag.glsl ${artifact_dir} && \
  ./${tools}sourcify-glsl.exe assets/glsl/normal_debug_frag.glsl ${artifact_dir} && \
  ./${tools}sourcify-glsl.exe assets/glsl/normal_debug_vert.glsl ${artifact_dir} && \
  ./${tools}sourcify-glsl.exe assets/glsl/normal_debug_geo.glsl ${artifact_dir} && \
  ./${tools}sourcify-glsl.exe assets/glsl/alpha_texture_frag.glsl ${artifact_dir} && \
  ./${tools}sourcify-glsl.exe assets/glsl/mountain_frag.glsl ${artifact_dir} \
  && \
  ./${tools}sourcify-png.exe assets/png/water.png 4 ${artifact_dir} && \
  ./${tools}sourcify-png.exe assets/png/cloud_cover.png 3 ${artifact_dir} && \
  ./${tools}sourcify-png.exe assets/png/stars.png 4 ${artifact_dir} && \
  ./${tools}sourcify-png.exe assets/png/mountain.png 4 ${artifact_dir} && \
  ./${tools}sourcify-png.exe assets/png/steam.png 4 ${artifact_dir} \
  && \
  ./${tools}sourcify-obj.exe assets/mesh/burdock.obj smooth ${artifact_dir} && \
  ./${tools}sourcify-obj.exe assets/mesh/pyramid.obj flat ${artifact_dir} && \
  ./${tools}sourcify-obj.exe assets/mesh/exclamation.obj flat ${artifact_dir} && \
  ./${tools}sourcify-obj.exe assets/mesh/capsule_apartment.obj smooth ${artifact_dir} && \
  ./${tools}sourcify-obj.exe assets/mesh/core.obj smooth ${artifact_dir} && \
  ./${tools}sourcify-obj.exe assets/mesh/cage.obj smooth ${artifact_dir} && \
  ./${tools}sourcify-obj.exe assets/mesh/sphere.obj smooth ${artifact_dir} && \
  ./${tools}sourcify-obj.exe assets/mesh/bird.obj flat ${artifact_dir} && \
  ./${tools}sourcify-obj.exe assets/mesh/turbine_base.obj flat ${artifact_dir} && \
  ./${tools}sourcify-obj.exe assets/mesh/turbine_blades.obj flat ${artifact_dir} && \
  ./${tools}sourcify-obj.exe assets/mesh/smooth_cube.obj flat ${artifact_dir} && \
  ./${tools}sourcify-obj.exe assets/mesh/mountain.obj smooth ${artifact_dir}
  # TODO: would be nice to get obj files outta these when building assets,
  # gotta figure out way to send obj files to obj dir in this step?
  # &&
  # i686-w64-mingw32-gcc \
  # ${options} \
  # ${includes} \
  # -c ${artifact_dir}*.c \
  # -o obj \
  # libs/tail/static/tail.a
}

ARG1=${@:$OPTIND:1}

if [ "$ARG1" == "clean" ]; then
  clean
elif [ "$ARG1" == "build-src" ]; then
  build_src
elif [ "$ARG1" == "build-assets" ]; then
  build_assets
elif [ "$ARG1" == "build-all" ]; then
  build_assets && build_src
else
  echo "You must specify build-assets or build-src"
fi
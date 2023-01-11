#!/bin/bash

target="i686-w64-mingw32-gcc"
tools="libs/tail/tools/"
artifact_dir="src/temp_asset_src_files/"
tail_static="libs/tail/static/tail.a"
declare -a options=("-O2" "-Wall")
declare -a includes=("-Ilibs/tail/include" "-Isrc/headers" \
"-Isrc/player" "-Isrc/scenes" "-Isrc/gameplay" "-Isrc/background" -I$artifact_dir)

declare -a src_directories=("src/" "src/background/" "src/gameplay/" "src/scenes/")

clean() {
  rm -rf obj bin $artifact_dir
}

build_assets() {

  rm -rf $artifact_dir obj
  mkdir $artifact_dir obj

  ./${tools}validate-glsl.exe assets/glsl/normal_debug_frag.glsl frag && \
  ./${tools}validate-glsl.exe assets/glsl/normal_debug_vert.glsl vert && \
  ./${tools}validate-glsl.exe assets/glsl/normal_debug_geo.glsl geo && \
  ./${tools}validate-glsl.exe assets/glsl/barebones_vert.glsl vert && \
  ./${tools}validate-glsl.exe assets/glsl/default_vert.glsl vert && \
  ./${tools}validate-glsl.exe assets/glsl/solid_color_frag.glsl frag && \
  ./${tools}validate-glsl.exe assets/glsl/flat_texture_frag.glsl frag && \
  ./${tools}validate-glsl.exe assets/glsl/water_surface_frag.glsl frag \
  ./${tools}validate-glsl.exe assets/glsl/sky_frag.glsl frag && \
  ./${tools}validate-glsl.exe assets/glsl/bouncer_frag.glsl frag && \
  ./${tools}validate-glsl.exe assets/glsl/steam_frag.glsl frag && \
  ./${tools}validate-glsl.exe assets/glsl/firing_guide_frag.glsl frag && \
  ./${tools}validate-glsl.exe assets/glsl/statue_frag.glsl frag && \
  ./${tools}validate-glsl.exe assets/glsl/mountain_frag.glsl frag \
  && \
  ./${tools}sourcify-glsl.exe assets/glsl/normal_debug_frag.glsl $artifact_dir && \
  ./${tools}sourcify-glsl.exe assets/glsl/normal_debug_vert.glsl $artifact_dir && \
  ./${tools}sourcify-glsl.exe assets/glsl/normal_debug_geo.glsl $artifact_dir && \
  ./${tools}sourcify-glsl.exe assets/glsl/flat_texture_frag.glsl $artifact_dir && \
  ./${tools}sourcify-glsl.exe assets/glsl/solid_color_frag.glsl $artifact_dir && \
  ./${tools}sourcify-glsl.exe assets/glsl/default_vert.glsl $artifact_dir && \
  ./${tools}sourcify-glsl.exe assets/glsl/barebones_vert.glsl $artifact_dir && \
  ./${tools}sourcify-glsl.exe assets/glsl/water_surface_frag.glsl $artifact_dir && \
  ./${tools}sourcify-glsl.exe assets/glsl/sky_frag.glsl $artifact_dir && \
  ./${tools}sourcify-glsl.exe assets/glsl/bouncer_frag.glsl $artifact_dir && \
  ./${tools}sourcify-glsl.exe assets/glsl/steam_frag.glsl $artifact_dir && \
  ./${tools}sourcify-glsl.exe assets/glsl/firing_guide_frag.glsl $artifact_dir && \
  ./${tools}sourcify-glsl.exe assets/glsl/statue_frag.glsl $artifact_dir && \
  ./${tools}sourcify-glsl.exe assets/glsl/mountain_frag.glsl $artifact_dir \
  && \
  ./${tools}sourcify-png.exe assets/png/water.png 3 $artifact_dir && \
  ./${tools}sourcify-png.exe assets/png/mountain.png 3 $artifact_dir && \
  ./${tools}sourcify-png.exe assets/png/clouds.png 3 $artifact_dir && \
  ./${tools}sourcify-png.exe assets/png/fireball.png 3 $artifact_dir && \
  ./${tools}sourcify-png.exe assets/png/statue.png 3 $artifact_dir && \
  ./${tools}sourcify-png.exe assets/png/steam.png 3 $artifact_dir \
  && \
  ./${tools}sourcify-obj.exe assets/mesh/pyramid.obj $artifact_dir && \
  ./${tools}sourcify-obj.exe assets/mesh/exclamation.obj $artifact_dir && \
  ./${tools}sourcify-obj.exe assets/mesh/sphere.obj $artifact_dir && \
  ./${tools}sourcify-obj.exe assets/mesh/bird.obj $artifact_dir && \
  ./${tools}sourcify-obj.exe assets/mesh/lowpoly_sphere_flat.obj $artifact_dir && \
  ./${tools}sourcify-obj.exe assets/mesh/lowpoly_sphere_smooth.obj $artifact_dir && \
  ./${tools}sourcify-obj.exe assets/mesh/smooth_cube.obj $artifact_dir && \
  ./${tools}sourcify-obj.exe assets/mesh/sky_cylinder.obj $artifact_dir && \
  ./${tools}sourcify-obj.exe assets/mesh/angel.obj $artifact_dir && \
  ./${tools}sourcify-obj.exe assets/mesh/mountain.obj $artifact_dir

  for filepath in ${artifact_dir}*.c; do
    echo $filepath
    src_file=$(basename $filepath .c)
    $target -c $artifact_dir$src_file.c -o obj/$src_file.o ${includes[@]} ${options[@]}
  done
}

compile_src() {
  for src_dir in ${src_directories[@]}; do
    for filepath in ${src_dir}*.c; do
      src_file=$(basename $filepath .c)
      $target -c $src_dir$src_file.c -o obj/$src_file.o ${includes[@]} ${options[@]}
    done
  done
}

link_obj() {
  rm -rf bin
  mkdir bin
  $target -o bin/bullgame.exe $(echo obj/*.o) $tail_static
}

ARG1=${@:$OPTIND:1}

if [ "$ARG1" == "clean" ]; then
  clean
elif [ "$ARG1" == "assets" ]; then
  clean && build_assets
elif [ "$ARG1" == "src" ]; then
  compile_src && link_obj
elif [ "$ARG1" == "all" ]; then
  clean && build_assets && compile_src && link_obj
else
  echo "You must specify src, assets, or all"
fi
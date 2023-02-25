#ifndef __BULL_ASSETS__
#define __BULL_ASSETS__

#include "tail.h"

#define TEXTURE_COUNT 13

#define BLASTED_STONE_TEXTURE 1 << 0
#define BULLETS_TEXTURE 1 << 1
#define CLOUDS_TEXTURE 1 << 2
#define CONCRETE_WALL_TEXTURE 1 << 3
#define FIREBALL_TEXTURE 1 << 4
#define MIST_TEXTURE 1 << 5
#define STEAM_TEXTURE 1 << 6
#define WATER_TEXTURE 1 << 7
#define BLINK_TEXTURE 1 << 8
#define MOUNTAIN_TEXTURE 1 << 9
#define DARK_RUST_TEXTURE 1 << 10
#define SMALL_SPARK_TEXTURE 1 << 11
#define ARROW_LAUNCHER_TEXTURE 1 << 12

extern struct Texture* TEXTURES[TEXTURE_COUNT];

void assets__copy_textures_to_gpu(
  struct GPU const *const gpu
);

#endif
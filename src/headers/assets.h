#ifndef __BULL_ASSETS__
#define __BULL_ASSETS__

#include "tail.h"

#define TEXTURE_COUNT 12

enum texture_key {
  BLASTED_STONE_TEXTURE,
  BULLETS_TEXTURE,
  CLOUDS_TEXTURE,
  CONCRETE_WALL_TEXTURE,
  FIREBALL_TEXTURE,
  MIST_TEXTURE,
  STEAM_TEXTURE,
  WATER_TEXTURE,
  BLINK_TEXTURE,
  MOUNTAIN_TEXTURE,
  DARK_RUST_TEXTURE,
  SMALL_SPARK_TEXTURE
};

extern struct Texture* TEXTURES[TEXTURE_COUNT];

void assets__copy_textures_to_gpu(
  struct GPU const *const gpu
);

#endif
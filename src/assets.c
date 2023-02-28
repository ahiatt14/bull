#include <stdio.h>

#include "tail.h"

#include "assets.h"

#include "blasted_stone_texture.h"
#include "bullets_texture.h"
#include "clouds_texture.h"
#include "concrete_wall_texture.h"
#include "fireball_texture.h"
#include "mist_texture.h"
#include "steam_texture.h"
#include "water_texture.h"
#include "blink_texture.h"
#include "mountain_texture.h"
#include "dark_rust_texture.h"
#include "small_spark_texture.h"
#include "arrow_texture.h"

// TODO: I don't like this naming "tail_texture" vs "texture" for the bitmask vals
struct Texture* TEXTURES[TEXTURE_COUNT] = {
  &BLASTED_STONE_TAIL_TEXTURE,
  &BULLETS_TAIL_TEXTURE,
  &CLOUDS_TAIL_TEXTURE,
  &CONCRETE_WALL_TAIL_TEXTURE,
  &FIREBALL_TAIL_TEXTURE,
  &MIST_TAIL_TEXTURE,
  &STEAM_TAIL_TEXTURE,
  &WATER_TAIL_TEXTURE,
  &BLINK_TAIL_TEXTURE,
  &MOUNTAIN_TAIL_TEXTURE,
  &DARK_RUST_TAIL_TEXTURE,
  &SMALL_SPARK_TAIL_TEXTURE,
  &ARROW_TAIL_TEXTURE
};

void assets__copy_textures_to_gpu(
  struct GPU const *const gpu
) {
  gpu->copy_texture_to_gpu(FILTER__NEAREST, &BLASTED_STONE_TAIL_TEXTURE);
  gpu->copy_texture_to_gpu(FILTER__NEAREST, &BULLETS_TAIL_TEXTURE);
  gpu->copy_texture_to_gpu(FILTER__NEAREST, &CLOUDS_TAIL_TEXTURE);
  gpu->copy_texture_to_gpu(FILTER__NEAREST, &CONCRETE_WALL_TAIL_TEXTURE);
  gpu->copy_texture_to_gpu(FILTER__NEAREST, &FIREBALL_TAIL_TEXTURE);
  gpu->copy_texture_to_gpu(FILTER__LINEAR, &MIST_TAIL_TEXTURE);
  gpu->copy_texture_to_gpu(FILTER__NEAREST, &STEAM_TAIL_TEXTURE);
  gpu->copy_texture_to_gpu(FILTER__LINEAR, &WATER_TAIL_TEXTURE);
  gpu->copy_texture_to_gpu(FILTER__NEAREST, &BLINK_TAIL_TEXTURE);
  gpu->copy_texture_to_gpu(FILTER__LINEAR, &MOUNTAIN_TAIL_TEXTURE);
  gpu->copy_texture_to_gpu(FILTER__NEAREST, &DARK_RUST_TAIL_TEXTURE);
  gpu->copy_texture_to_gpu(FILTER__NEAREST, &SMALL_SPARK_TAIL_TEXTURE);
  gpu->copy_texture_to_gpu(FILTER__NEAREST, &ARROW_TAIL_TEXTURE);
}
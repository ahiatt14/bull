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
  &DARK_RUST_TAIL_TEXTURE
};

void assets__copy_textures_to_gpu(
  struct GPU const *const gpu
) {
  gpu->copy_texture_to_gpu(TEXTURES[BLASTED_STONE_TEXTURE]);
  gpu->copy_texture_to_gpu(TEXTURES[BULLETS_TEXTURE]);
  gpu->copy_texture_to_gpu(TEXTURES[CLOUDS_TEXTURE]);
  gpu->copy_texture_to_gpu(TEXTURES[CONCRETE_WALL_TEXTURE]);
  gpu->copy_texture_to_gpu(TEXTURES[FIREBALL_TEXTURE]);
  gpu->copy_texture_to_gpu(TEXTURES[MIST_TEXTURE]);
  gpu->copy_texture_to_gpu(TEXTURES[STEAM_TEXTURE]);
  gpu->copy_texture_to_gpu(TEXTURES[WATER_TEXTURE]);
  gpu->copy_texture_to_gpu(TEXTURES[BLINK_TEXTURE]);
  gpu->copy_texture_to_gpu(TEXTURES[MOUNTAIN_TEXTURE]);
  gpu->copy_texture_to_gpu(TEXTURES[DARK_RUST_TEXTURE]);
}
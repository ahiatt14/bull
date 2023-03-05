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
#include "blue_pulse_texture.h"
#include "mountain_texture.h"
#include "dark_rust_texture.h"
#include "small_spark_texture.h"
#include "arrow_texture.h"
#include "muzzle_flash_texture.h"
#include "cooling_tower_light_texture.h"

#include "ocean_sky_nx_texture.h"
#include "ocean_sky_px_texture.h"
#include "ocean_sky_ny_texture.h"
#include "ocean_sky_py_texture.h"
#include "ocean_sky_nz_texture.h"
#include "ocean_sky_pz_texture.h"

// TODO: I don't like this naming "tail_texture" vs "texture" for the bitmask vals
Texture* TEXTURES[TEXTURE_COUNT] = {
  &BLASTED_STONE_TAIL_TEXTURE,
  &BULLETS_TAIL_TEXTURE,
  &CLOUDS_TAIL_TEXTURE,
  &CONCRETE_WALL_TAIL_TEXTURE,
  &FIREBALL_TAIL_TEXTURE,
  &MIST_TAIL_TEXTURE,
  &STEAM_TAIL_TEXTURE,
  &WATER_TAIL_TEXTURE,
  &BLUE_PULSE_TAIL_TEXTURE,
  &MOUNTAIN_TAIL_TEXTURE,
  &DARK_RUST_TAIL_TEXTURE,
  &SMALL_SPARK_TAIL_TEXTURE,
  &ARROW_TAIL_TEXTURE,
  &MUZZLE_FLASH_TAIL_TEXTURE,
  &COOLING_TOWER_LIGHT_TAIL_TEXTURE
};

Cubemap OCEAN_SKYBOX = {
  .sides = (Texture*[6]){
    &OCEAN_SKY_PX_TAIL_TEXTURE,
    &OCEAN_SKY_NX_TAIL_TEXTURE,
    &OCEAN_SKY_PY_TAIL_TEXTURE,
    &OCEAN_SKY_NY_TAIL_TEXTURE,
    &OCEAN_SKY_PZ_TAIL_TEXTURE,
    &OCEAN_SKY_NZ_TAIL_TEXTURE
  }
};

void assets__copy_textures_to_gpu(
  GPU const *const gpu
) {
  gpu->copy_texture_to_gpu(FILTER__NEAREST, WRAP__REPEAT, &BLASTED_STONE_TAIL_TEXTURE);
  gpu->copy_texture_to_gpu(FILTER__NEAREST, WRAP__REPEAT, &BULLETS_TAIL_TEXTURE);
  gpu->copy_texture_to_gpu(FILTER__NEAREST, WRAP__REPEAT, &CLOUDS_TAIL_TEXTURE);
  gpu->copy_texture_to_gpu(FILTER__NEAREST, WRAP__REPEAT, &CONCRETE_WALL_TAIL_TEXTURE);
  gpu->copy_texture_to_gpu(FILTER__NEAREST, WRAP__REPEAT, &FIREBALL_TAIL_TEXTURE);
  gpu->copy_texture_to_gpu(FILTER__LINEAR, WRAP__REPEAT, &MIST_TAIL_TEXTURE);
  gpu->copy_texture_to_gpu(FILTER__NEAREST, WRAP__REPEAT, &STEAM_TAIL_TEXTURE);
  gpu->copy_texture_to_gpu(FILTER__LINEAR, WRAP__REPEAT, &WATER_TAIL_TEXTURE);
  gpu->copy_texture_to_gpu(FILTER__LINEAR, WRAP__REPEAT, &BLUE_PULSE_TAIL_TEXTURE);
  gpu->copy_texture_to_gpu(FILTER__LINEAR, WRAP__REPEAT, &MOUNTAIN_TAIL_TEXTURE);
  gpu->copy_texture_to_gpu(FILTER__NEAREST, WRAP__REPEAT, &DARK_RUST_TAIL_TEXTURE);
  gpu->copy_texture_to_gpu(FILTER__NEAREST, WRAP__REPEAT, &SMALL_SPARK_TAIL_TEXTURE);
  gpu->copy_texture_to_gpu(FILTER__NEAREST, WRAP__REPEAT, &ARROW_TAIL_TEXTURE);
  gpu->copy_texture_to_gpu(FILTER__NEAREST, WRAP__CLAMP, &MUZZLE_FLASH_TAIL_TEXTURE);
  gpu->copy_texture_to_gpu(FILTER__NEAREST, WRAP__CLAMP, &COOLING_TOWER_LIGHT_TAIL_TEXTURE);

  gpu->copy_cubemap_to_gpu(FILTER__NEAREST, &OCEAN_SKYBOX);
}
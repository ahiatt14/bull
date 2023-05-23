#include <stdio.h>
#include <stdint.h>

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
#include "lcd_number_texture.h"
#include "wave_crest_texture.h"

#include "ocean_sky_nx_texture.h"
#include "ocean_sky_px_texture.h"
#include "ocean_sky_ny_texture.h"
#include "ocean_sky_py_texture.h"
#include "ocean_sky_nz_texture.h"
#include "ocean_sky_pz_texture.h"

uint_fast32_t LCD_NUMBER_TEXTURE = 1 << 0;
uint_fast32_t BULLETS_TEXTURE = 1 << 1;
uint_fast32_t CLOUDS_TEXTURE = 1 << 2;
uint_fast32_t CONCRETE_WALL_TEXTURE = 1 << 3;
uint_fast32_t FIREBALL_TEXTURE = 1 << 4;
uint_fast32_t MIST_TEXTURE = 1 << 5;
uint_fast32_t STEAM_TEXTURE = 1 << 6;
uint_fast32_t WATER_TEXTURE = 1 << 7;
uint_fast32_t BLUE_PULSE_TEXTURE = 1 << 8;
uint_fast32_t MOUNTAIN_TEXTURE = 1 << 9;
uint_fast32_t DARK_RUST_TEXTURE = 1 << 10;
uint_fast32_t SMALL_SPARK_TEXTURE = 1 << 11;
uint_fast32_t ARROW_TEXTURE = 1 << 12;
uint_fast32_t MUZZLE_FLASH_TEXTURE = 1 << 13;
uint_fast32_t COOLING_TOWER_LIGHT_TEXTURE = 1 << 14;
uint_fast32_t BLASTED_STONE_TEXTURE = 1 << 15;
uint_fast32_t WAVE_CREST_TEXTURE = 1 << 16;

// TODO: I don't like this naming "tail_texture" vs "texture" for the bitmask vals
Texture* TEXTURES[TEXTURE_COUNT] = {
  &LCD_NUMBER_TAIL_TEXTURE,
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
  &COOLING_TOWER_LIGHT_TAIL_TEXTURE,
  &BLASTED_STONE_TAIL_TEXTURE,
  &WAVE_CREST_TAIL_TEXTURE
};

Cubemap OCEAN_SKYBOX = {
  .sides = (Texture*[6]){
    &OCEAN_SKY_PX_TAIL_TEXTURE,
    &OCEAN_SKY_NX_TAIL_TEXTURE,
    &OCEAN_SKY_NY_TAIL_TEXTURE,
    &OCEAN_SKY_PY_TAIL_TEXTURE,
    &OCEAN_SKY_PZ_TAIL_TEXTURE,
    &OCEAN_SKY_NZ_TAIL_TEXTURE
  }
};
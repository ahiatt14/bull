#ifndef __BULL_ASSETS__
#define __BULL_ASSETS__

#include "tail.h"

#define TEXTURE_COUNT 16

#define BLASTED_STONE_TEXTURE 1 << 0
#define BULLETS_TEXTURE 1 << 1
#define CLOUDS_TEXTURE 1 << 2
#define CONCRETE_WALL_TEXTURE 1 << 3
#define FIREBALL_TEXTURE 1 << 4
#define MIST_TEXTURE 1 << 5
#define STEAM_TEXTURE 1 << 6
#define WATER_TEXTURE 1 << 7
#define BLUE_PULSE_TEXTURE 1 << 8
#define MOUNTAIN_TEXTURE 1 << 9
#define DARK_RUST_TEXTURE 1 << 10
#define SMALL_SPARK_TEXTURE 1 << 11
#define ARROW_TEXTURE 1 << 12
#define MUZZLE_FLASH_TEXTURE 1 << 13
#define COOLING_TOWER_LIGHT_TEXTURE 1 << 14
#define LCD_NUMBER_TEXTURE 1 << 15

extern Cubemap OCEAN_SKYBOX;

extern Texture* TEXTURES[TEXTURE_COUNT];

void assets__copy_textures_to_gpu(
  GPU const *const gpu
);

#endif
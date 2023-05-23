#ifndef __BULL_ASSETS__
#define __BULL_ASSETS__

#include <stdint.h>

#include "tail.h"

#define TEXTURE_COUNT 17

extern uint_fast32_t BLASTED_STONE_TEXTURE;
extern uint_fast32_t BULLETS_TEXTURE;
extern uint_fast32_t CLOUDS_TEXTURE;
extern uint_fast32_t CONCRETE_WALL_TEXTURE;
extern uint_fast32_t FIREBALL_TEXTURE;
extern uint_fast32_t MIST_TEXTURE;
extern uint_fast32_t STEAM_TEXTURE;
extern uint_fast32_t WATER_TEXTURE;
extern uint_fast32_t BLUE_PULSE_TEXTURE;
extern uint_fast32_t MOUNTAIN_TEXTURE;
extern uint_fast32_t DARK_RUST_TEXTURE;
extern uint_fast32_t SMALL_SPARK_TEXTURE;
extern uint_fast32_t ARROW_TEXTURE;
extern uint_fast32_t MUZZLE_FLASH_TEXTURE;
extern uint_fast32_t COOLING_TOWER_LIGHT_TEXTURE;
extern uint_fast32_t LCD_NUMBER_TEXTURE;
extern uint_fast32_t WAVE_CREST_TEXTURE;

extern Cubemap OCEAN_SKYBOX;

extern Texture* TEXTURES[TEXTURE_COUNT];

#endif
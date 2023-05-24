#ifndef __BULL_ASSETS__
#define __BULL_ASSETS__

#include <stdint.h>

#include "tail.h"

// STATIC MESHES
#include "player_body_mesh.h"
#include "player_left_leg_mesh.h"
#include "player_right_leg_mesh.h"
#include "drone_mesh.h"
#include "rocket_mesh.h"
#include "lcd_number_mesh.h"
#include "lowpoly_sphere_flat_mesh.h"
#include "lowpoly_sphere_smooth_mesh.h"
#include "exclamation_mesh.h"
#include "lowpoly_mushroom_cloud_mesh.h"
#include "cooling_tower_mesh.h"
#include "cloud_wall_mesh.h"
#include "mountain_mesh.h"
#include "arrow_mesh.h"
#include "cubemap_mesh.h"
#include "tower_discharges_mesh.h"
#include "tower_pipes_mesh.h"
#include "cloud_cover_mesh.h"
#include "muzzle_flash_mesh.h"

// DYNAMIC MESHES
#include "steam_column_mesh.h"

#define TEXTURE_COUNT 17
#define STATIC_MESH_COUNT 20
#define DYNAMIC_MESH_COUNT 1

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

extern PointBuffer POINT;
extern DrawableMesh QUAD;

#endif
#include <stdint.h>
#include <stdio.h>

#include "tail.h"

#include "scene.h"

#include "assets.h"
#include "colors.h"

#define ASSETS_TO_COPY TEXTURE_COUNT + STATIC_MESH_COUNT + DYNAMIC_MESH_COUNT

static DrawableMesh *static_meshes[STATIC_MESH_COUNT];
static DrawableMesh *dynamic_meshes[DYNAMIC_MESH_COUNT];

static uint_fast8_t textures_copied;
static uint_fast8_t static_meshes_copied;
static uint_fast8_t dynamic_meshes_copied;

void loading__init(
  Window const *const window,
  Viewport *const vwprt,
  GPU const *const gpu
) {

  static_meshes[0] = &PLAYER_BODY_MESH;
  static_meshes[1] = &PLAYER_LEFT_LEG_MESH;
  static_meshes[2] = &PLAYER_RIGHT_LEG_MESH;
  static_meshes[3] = &DRONE_MESH;
  static_meshes[4] = &ROCKET_MESH;
  static_meshes[5] = &LCD_NUMBER_MESH;
  static_meshes[6] = &LOWPOLY_MUSHROOM_CLOUD_MESH;
  static_meshes[7] = &LOWPOLY_SPHERE_FLAT_MESH;
  static_meshes[8] = &LOWPOLY_SPHERE_SMOOTH_MESH;
  static_meshes[9] = &EXCLAMATION_MESH;
  static_meshes[10] = &LOWPOLY_MUSHROOM_CLOUD_MESH;
  static_meshes[11] = &COOLING_TOWER_MESH;
  static_meshes[12] = &MOUNTAIN_MESH;
  static_meshes[13] = &ARROW_MESH;
  static_meshes[14] = &CUBEMAP_MESH;
  static_meshes[15] = &TOWER_DISCHARGES_MESH;
  static_meshes[16] = &TOWER_PIPES_MESH;
  static_meshes[17] = &CLOUD_COVER_MESH;
  static_meshes[18] = &MUZZLE_FLASH_MESH;
  static_meshes[19] = &CLOUD_WALL_MESH;

  dynamic_meshes[0] = &STEAM_COLUMN_MESH;

  gpu->copy_static_mesh_to_gpu(&QUAD);

  gpu->copy_points_to_gpu(&POINT);

  gpu->copy_cubemap_to_gpu(FILTER__NEAREST, &OCEAN_SKYBOX);

  gpu->copy_texture_to_gpu(FILTER__NEAREST, WRAP__REPEAT, TEXTURES[0]);
  textures_copied++;
}

void loading__tick(
  GameTime time,
  Window const *const window,
  Viewport *const vwprt,
  GPU const *const gpu,
  uint8_t previous_scene,
  void (*switch_scene)(uint8_t new_scene)
) {

  if (textures_copied < TEXTURE_COUNT) {

    gpu->copy_texture_to_gpu(
      FILTER__NEAREST,
      WRAP__REPEAT,
      TEXTURES[textures_copied]
    );
    textures_copied++;

  } else if (static_meshes_copied < STATIC_MESH_COUNT) {

    gpu->copy_static_mesh_to_gpu(static_meshes[static_meshes_copied]);
    static_meshes_copied++;

  } else if (dynamic_meshes_copied < DYNAMIC_MESH_COUNT) {

    gpu->copy_dynamic_mesh_to_gpu(dynamic_meshes[dynamic_meshes_copied]);
    dynamic_meshes_copied++;
    
  }

  uint_fast16_t assets_copied =
    textures_copied +
    static_meshes_copied +
    dynamic_meshes_copied;

  if (assets_copied == ASSETS_TO_COPY) switch_scene(SCENE__ACTION);

  gpu->clear(&COLOR_BLUE);
}
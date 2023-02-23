#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "tail.h"

#include "ecs.h"

#include "scene.h"

#include "assets.h"
#include "constants.h"
#include "bull_math.h"
#include "tail_helpers.h"

#include "cooling_tower_mesh.h"

#include "water.h"

#include "mountain_mesh.h"
#include "mountain_frag.h"
#include "default_vert.h"

#include "flat_texture_frag.h"
#include "steam_column_mesh.h"
#include "steam_frag.h"
#include "steam_geo.h"

#include "billboard_geo.h"
#include "billboard_vert.h"

#include "sky_mesh.h"
#include "mist_frag.h"

// CONSTANTS

#define CAMERA_ROTATE_SPEED 0

// READ ONLY

static const struct Transform COOLING_TOWER_TRANSFORM = {
  .position = { -17, 0.3f, -40 },
  .scale = 5,
  .rotation = (struct Quaternion){0}
};

// static const struct Vec3 COOLING_TOWER_OFFSET = {
//   7,
//   0,
//   4
// };

// LOCALS

static struct Camera cam;
static EntityId waves;

// COOLING TOWER

static struct Shader cooling_tower_shader;
static struct M4x4 cooling_tower_local_to_world;
static struct M3x3 cooling_tower_normals_local_to_world;

// MOUNTAINS

static struct Shader mountain_shader;
static struct M4x4 mountain_local_to_world;
static struct M3x3 mountain_normals_local_to_world;
static struct Transform mountain_transform;

// STEAM

static struct Shader steam_shader;
static struct M4x4 steam_local_to_world;
static struct M3x3 steam_normals_local_to_world;

// SKY

static struct Vec3 sunlight_direction = {
  -1, 0, -1
};

static struct Transform sky_transform = {
  .position = { 0, 7, -60 },
  .scale = 50
};
static struct M4x4 sky_local_to_world;
static struct Shader sky_shader;

// MIST

static struct ECS ecs;
static EntityId mist;
static struct Shader mist_shader;

void ocean__init(
  struct Window const *const window,
  struct Viewport *const vwprt,
  struct GPU const *const gpu
) {

  cam.position = (struct Vec3){ 0, 2, 12 };
  cam.look_target = (struct Vec3){ 0, 2, 0 };
  cam.horizontal_fov_in_deg = 60;
  cam.near_clip_distance = 0.3f;
  cam.far_clip_distance = 100;
  camera__calculate_lookat(WORLDSPACE.up, &cam);
  camera__calculate_perspective(vwprt, &cam);

  // STEAM

  steam_shader.frag_src = FLAT_TEXTURE_FRAG_SRC;
  steam_shader.geo_src = STEAM_GEO_SRC;
  steam_shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&steam_shader);
  gpu->copy_dynamic_mesh_to_gpu(&STEAM_COLUMN_MESH);
  space__create_model(
    &WORLDSPACE,
    &COOLING_TOWER_TRANSFORM,
    &steam_local_to_world
  );
  space__create_normals_model(
    &steam_local_to_world,
    &steam_normals_local_to_world
  );

  // SKY

  sky_shader.frag_src = FLAT_TEXTURE_FRAG_SRC;
  sky_shader.geo_src = BILLBOARD_GEO_SRC;
  sky_shader.vert_src = BILLBOARD_VERT_SRC;
  gpu->copy_shader_to_gpu(&sky_shader);
  m4x4__translation(
    sky_transform.position,
    &sky_local_to_world
  );

  // OCEAN
  water__copy_assets_to_gpu(gpu);
  waves = create_water(&ecs);

  // MIST
  mist_shader.frag_src = MIST_FRAG_SRC;
  mist_shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&mist_shader);
  gpu->copy_static_mesh_to_gpu(&SKY_MESH);
  mist = ecs__create_entity(&ecs);
  ecs__add_transform(
    mist,
    (struct Transform){
      .scale = 12,
      .rotation = (struct Quaternion){0},
      .position = (struct Vec3){ 1, 4, 2 }
    },
    &ecs
  );
  ecs__add_uv_scroll(
    mist,
    (struct ScrollUV){
      .speed = (struct Vec2){ -0.01f, 0 },
      .total = (struct Vec2){0}
    },
    &ecs
  );
  ecs__add_draw(
    mist,
    (struct Draw){
      .shader = &mist_shader,
      .texture = TEXTURES[MIST_TEXTURE],
      .mesh = &SKY_MESH,
      .draw = ecs__draw_mesh
    },
    &ecs
  );

  // COOLING TOWER

  cooling_tower_shader.frag_src = MOUNTAIN_FRAG_SRC;
  cooling_tower_shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&cooling_tower_shader);
  gpu->copy_static_mesh_to_gpu(&COOLING_TOWER_MESH);
  space__create_model(
    &WORLDSPACE,
    &COOLING_TOWER_TRANSFORM,
    &cooling_tower_local_to_world
  );
  space__create_normals_model(
    &cooling_tower_local_to_world,
    &cooling_tower_normals_local_to_world
  );

  // MOUNTAINS

  mountain_transform = (struct Transform){
    .position = { 10, -0.4f, -50 },
    .rotation = quaternion__create(
      (struct Vec3){ 0, 1, 0 },
      -M_PI * 0.25f
    ),
    .scale = 13
  };
  mountain_shader.frag_src = MOUNTAIN_FRAG_SRC;
  mountain_shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&mountain_shader);
  mesh__tile_uvs(2, 2, &MOUNTAIN_MESH);
  gpu->copy_static_mesh_to_gpu(&MOUNTAIN_MESH);
  space__create_model(
    &WORLDSPACE,
    &mountain_transform,
    &mountain_local_to_world
  );
  space__create_normals_model(
    &mountain_local_to_world,
    &mountain_normals_local_to_world
  );
}

void ocean__tick(
  struct GameTime time, 
  struct Window const *const window,
  struct Viewport *const vwprt,
  struct GPU const *const gpu,
  uint8_t previous_scene,
  void (*switch_scene)(uint8_t new_scene)
) {

  // UPDATE

  for (unsigned int i = 0; i < STEAM_COLUMN_MESH.vertices_length; i++) {
    STEAM_COLUMN_MESH.vertices[i].uv.x += 0.06f * time.delta;
  }
  gpu->update_gpu_mesh_data(&STEAM_COLUMN_MESH);

  // cam.position.y += 0.5f * time.delta;
  // camera__calculate_lookat(WORLDSPACE.up, &cam);
  // camera__calculate_perspective(vwprt, &cam);

  ecs__scroll_uvs(time, &ecs);
  
  // DRAW

  gpu->cull_back_faces();

  // SKY

  gpu->select_shader(&sky_shader);
  gpu->select_texture(TEXTURES[CLOUDS_TEXTURE]);
  gpu__set_mvp(
    &sky_local_to_world,
    &M3X3_IDENTITY,
    &cam,
    &sky_shader,
    gpu
  );
  gpu->set_shader_float(
    &sky_shader,
    "scale",
    sky_transform.scale
  );
  gpu->draw_points(&POINT);

  // COOLING TOWER

  gpu->select_shader(&cooling_tower_shader);
  gpu->select_texture(TEXTURES[CONCRETE_WALL_TEXTURE]);
  gpu->set_shader_vec3(
    &cooling_tower_shader,
    "light_dir",
    vec3__normalize(sunlight_direction)
  );
  gpu->set_shader_vec3(
    &cooling_tower_shader,
    "light_color",
    COLOR_EVENING_SUNLIGHT
  );
  gpu__set_mvp(
    &cooling_tower_local_to_world,
    &cooling_tower_normals_local_to_world,
    &cam,
    &cooling_tower_shader,
    gpu
  );
  gpu->draw_mesh(&COOLING_TOWER_MESH);

  // MOUNTAINS

  gpu->select_shader(&mountain_shader);
  gpu->select_texture(TEXTURES[MOUNTAIN_TEXTURE]);
  gpu->set_shader_vec3(
    &mountain_shader,
    "light_dir",
    vec3__normalize(sunlight_direction)
  );
  gpu->set_shader_vec3(
    &mountain_shader,
    "light_color",
    COLOR_EVENING_SUNLIGHT
  );
  gpu__set_mvp(
    &mountain_local_to_world,
    &mountain_normals_local_to_world,
    &cam,
    &mountain_shader,
    gpu
  );
  gpu->draw_mesh(&MOUNTAIN_MESH);

  // STEAM

  gpu->select_shader(&steam_shader);
  gpu->select_texture(TEXTURES[STEAM_TEXTURE]);
  gpu->set_shader_float(
    &steam_shader,
    "speed",
    -0.36f
  );
  gpu->set_shader_float(
    &steam_shader,
    "seconds_since_activation",
    time.sec_since_game_launch
  );
  gpu->set_shader_float(
    &steam_shader,
    "max_altitude",
    2
  );
  gpu__set_mvp(
    &steam_local_to_world,
    &steam_normals_local_to_world,
    &cam,
    &steam_shader,
    gpu
  );
  gpu->draw_mesh(&STEAM_COLUMN_MESH);

  ecs__draw(
    time,
    &cam,
    gpu,
    &ecs
  );
}
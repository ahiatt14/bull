#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "tail.h"

#include "ecs.h"

#include "scene.h"
#include "constants.h"
#include "bull_math.h"
#include "tail_helpers.h"

#include "water.h"

#include "cooling_tower_mesh.h"
#include "concrete_wall_texture.h"

#include "mountain_mesh.h"
#include "mountain_texture.h"
#include "mountain_frag.h"
#include "default_vert.h"

#include "flat_texture_frag.h"
#include "steam_column_mesh.h"
#include "steam_texture.h"
#include "steam_frag.h"
#include "steam_geo.h"

#include "billboard_geo.h"
#include "billboard_vert.h"
#include "night_sky_texture.h"
// #include "clouds_texture.h"

// CONSTANTS

#define CAMERA_ROTATE_SPEED 0

// READ ONLY

static const struct Vec3 COOLING_TOWER_POSITION = {
  -5,
  -1.5f,
  -8
};

static const struct Vec2 WIND_KM_PER_SEC = {
  -0.0075f,
  -0.01f
};

// LOCALS

static struct Camera cam;

// COOLING TOWER

static struct Shader cooling_tower_shader;
static struct M4x4 cooling_tower_local_to_world;
static struct M3x3 cooling_tower_normals_local_to_world;
static struct Transform cooling_tower_transform;

// MOUNTAINS

static struct Shader mountain_shader;
static struct M4x4 mountain_local_to_world;
static struct M3x3 mountain_normals_local_to_world;
static struct Transform mountain_transform;

// STEAM

static struct Shader steam_shader;
static struct M4x4 steam_local_to_world;
static struct M3x3 steam_normals_local_to_world;
static struct Transform steam_transform;

// SKY

static struct Vec3 light_direction = {
  3, 0, 1
};

static struct Transform sky_transform = {
  .position = { 0, 0, -45 },
  .scale = 30
};
static struct M4x4 sky_local_to_world;
static struct Shader sky_shader;

void ocean__init(
  struct Window const *const window,
  struct Viewport *const vwprt,
  struct GPU const *const gpu
) {

  cam.position = (struct Vec3){ 0, 0.05f, 7 };
  cam.look_target = (struct Vec3){ 0, 1.05, 0 };
  cam.horizontal_fov_in_deg = 60;
  cam.near_clip_distance = 0.3f;
  cam.far_clip_distance = 100;

  camera__calculate_lookat(WORLDSPACE.up, &cam);
  camera__calculate_perspective(vwprt, &cam);

  // WATER

  water__init_mesh_data();
  water__copy_assets_to_gpu(gpu);

  // STEAM

  steam_transform = (struct Transform){
    .position = COOLING_TOWER_POSITION,
    .scale = 3.0f
  };
  // steam_shader.frag_src = STEAM_FRAG_SRC;
  steam_shader.frag_src = FLAT_TEXTURE_FRAG_SRC;
  steam_shader.geo_src = STEAM_GEO_SRC;
  steam_shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&steam_shader);
  gpu->copy_dynamic_mesh_to_gpu(&STEAM_COLUMN_MESH);
  gpu->copy_texture_to_gpu(&STEAM_TEXTURE);
  space__create_model(
    &WORLDSPACE,
    &steam_transform,
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
  gpu->copy_texture_to_gpu(&NIGHT_SKY_TEXTURE);
  m4x4__translation(
    sky_transform.position,
    &sky_local_to_world
  );

  // COOLING TOWER

  cooling_tower_transform = (struct Transform){
    .position = vec3_plus_vec3(
      COOLING_TOWER_POSITION,
      (struct Vec3){ 0, 0.7f, 0 }
    ),
    .scale = 3
  };

  cooling_tower_shader.frag_src = MOUNTAIN_FRAG_SRC;
  cooling_tower_shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&cooling_tower_shader);
  gpu->copy_static_mesh_to_gpu(&COOLING_TOWER_MESH);
  gpu->copy_texture_to_gpu(&CONCRETE_WALL_TEXTURE);
  space__create_model(
    &WORLDSPACE,
    &cooling_tower_transform,
    &cooling_tower_local_to_world
  );
  space__create_normals_model(
    &cooling_tower_local_to_world,
    &cooling_tower_normals_local_to_world
  );

  // MOUNTAINS

  mountain_transform = (struct Transform){
    .position = { 20, -0.4f, -23 },
    .rotation = quaternion__create(
      (struct Vec3){ 0, 1, 0 },
      -M_PI * 0.25f
    ),
    .scale = 10
  };
  mountain_shader.frag_src = MOUNTAIN_FRAG_SRC;
  mountain_shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&mountain_shader);
  mesh__tile_uvs(2, 2, &MOUNTAIN_MESH);
  gpu->copy_static_mesh_to_gpu(&MOUNTAIN_MESH);
  gpu->copy_texture_to_gpu(&MOUNTAIN_TEXTURE);
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
    // STEAM_COLUMN_MESH.vertices[i].uv.y -= 0.11f * time.delta;
    STEAM_COLUMN_MESH.vertices[i].uv.x += 0.033f * time.delta;
  }
  gpu->update_gpu_mesh_data(&STEAM_COLUMN_MESH);

  water__update_waves(WIND_KM_PER_SEC, time, gpu);
  
  // DRAW

  // SKY

  gpu->select_shader(&sky_shader);
  gpu->select_texture(&NIGHT_SKY_TEXTURE);
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
  gpu->select_texture(&CONCRETE_WALL_TEXTURE);
  gpu->set_shader_vec3(
    &cooling_tower_shader,
    "light_dir",
    vec3__normalize(light_direction)
  );
  gpu->set_shader_vec3(
    &cooling_tower_shader,
    "light_color",
    COLOR_GOLDEN_YELLOW
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
  gpu->select_texture(&MOUNTAIN_TEXTURE);
  gpu->set_shader_vec3(
    &mountain_shader,
    "light_dir",
    vec3__normalize(light_direction)
  );
  gpu->set_shader_vec3(
    &mountain_shader,
    "light_color",
    COLOR_GOLDEN_YELLOW
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
  gpu->select_texture(&STEAM_TEXTURE);
  gpu->set_shader_vec3(
    &steam_shader,
    "bottom_color",
    COLOR_WHITE
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

  water__draw(&cam, gpu);
}
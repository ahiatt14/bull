#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "tail.h"

#include "scene.h"
#include "constants.h"
#include "bull_math.h"
#include "tail_helpers.h"
#include "water.h"
#include "steam.h"
#include "SKY_CYLINDER_MESH.h"
#include "MOUNTAIN_MESH.h"

#include "steam_texture.h"
#include "MOUNTAIN_TEXTURE.h"
#include "water_texture.h"
#include "CLOUDS_TEXTURE.h"

#include "mountain_frag.h"
#include "default_vert.h"
#include "water_surface_frag.h"
#include "sky_frag.h"

// CONSTANTS

#define CAMERA_ROTATE_SPEED 0

// READ ONLY

static const struct Vec2 WIND_KM_PER_SEC = {
  -0.0075f,
  -0.01f
};

// FORWARD DECS

// LOCALS

static struct Vec3 steam_light_direction = { 1, 0, 0 };

// SKY

static struct Shader sky_shader;
static struct M4x4 sky_local_to_world;
static struct M3x3 sky_normals_local_to_world;
static struct Transform sky_transform;

// MOUNTAINS

static struct Shader mountain_shader;
static struct M4x4 mountain_local_to_world;
static struct M3x3 mountain_normals_local_to_world;
static struct Transform mountain_transform;

// STEAM

static struct steam_column steam0 = (struct steam_column){
  .position = { 4, -1.5, -15 },
  .shape_index_offset = 2
};
static struct steam_column steam1 = (struct steam_column){
  .position = { -10, -1.5, -6},
  .shape_index_offset = 7
};
static struct steam_column steam2 = (struct steam_column){
  .position = { 7, -1.5, 4 },
  .shape_index_offset = 14
};

static struct Camera cam;

void ocean__init(
  struct Window const *const window,
  struct Viewport *const vwprt,
  struct GPU const *const gpu
) {

  cam.position = (struct Vec3){ 0, 0.05, 7 };
  cam.look_target = (struct Vec3){ 0, 1.05, 0 };
  cam.horizontal_fov_in_deg = 60;
  cam.near_clip_distance = 0.3f;
  cam.far_clip_distance = 100;

  // WATER

  water__init_mesh_data();
  water__copy_assets_to_gpu(gpu);

  // STEAM

  steam__create_shared_mesh_data();
  steam__column_default(&steam0);
  steam__column_default(&steam1);
  steam__column_default(&steam2);
  steam__copy_assets_to_gpu(gpu);

  // SKY

  sky_transform = (struct Transform){
    { 0, -1, 0 },
    quaternion__create(WORLDSPACE.up, M_PI),
    32
  };

  sky_shader.frag_src = SKY_FRAG_SRC;
  sky_shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&sky_shader);
  mesh__tile_uvs(3, 3, &SKY_CYLINDER_MESH);
  gpu->copy_static_mesh_to_gpu(&SKY_CYLINDER_MESH);
  gpu->copy_texture_to_gpu(&CLOUDS_TEXTURE);
  space__create_model(
    &WORLDSPACE,
    &sky_transform,
    &sky_local_to_world
  );
  space__create_normals_model(
    &sky_local_to_world,
    &sky_normals_local_to_world
  );

  // MOUNTAINS

  mountain_transform = (struct Transform){
    .position = { 7, -0.2f, -18 },
    .rotation = quaternion__create(
      (struct Vec3){ 0, 1, 0 },
      deg_to_rad(300)
    ),
    .scale = 4
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

  static struct M4x4 camera_rotation;
  m4x4__rotation(
    deg_to_rad(time.delta * CAMERA_ROTATE_SPEED),
    WORLDSPACE.up,
    &camera_rotation
  );
  cam.position = m4x4_x_point(&camera_rotation, cam.position);
  steam_light_direction = m4x4_x_point(&camera_rotation, steam_light_direction);

  static struct Vec3 norm_steam_light_direction;
  norm_steam_light_direction = vec3__normalize(steam_light_direction);

  water__update_waves(WIND_KM_PER_SEC, time, gpu);

  steam__rise(time.delta, &steam0);
  steam__rise(time.delta, &steam1);
  steam__rise(time.delta, &steam2);
  
  // DRAW

  camera__calculate_lookat(WORLDSPACE.up, &cam);
  camera__calculate_perspective(vwprt, &cam);

  // SKY
  
  // TODO: go invert the sky cylinder normals in blender ya shmole
  gpu->cull_no_faces();

  // TODO: draw wireframe to help debug

  gpu->select_shader(&sky_shader);
  gpu->select_texture(&CLOUDS_TEXTURE);
  gpu->set_shader_vec3(&sky_shader, "horizon_color", COLOR_DARK_SLATE_GREY);
  gpu__set_mvp(
    &sky_local_to_world,
    &sky_normals_local_to_world,
    &cam,
    &sky_shader,
    gpu
  );
  gpu->draw_mesh(&SKY_CYLINDER_MESH);

  gpu->cull_back_faces();

  // MOUNTAINS

  gpu->select_shader(&mountain_shader);
  gpu->select_texture(&MOUNTAIN_TEXTURE);
  gpu->set_shader_vec3(
    &mountain_shader,
    "light_dir",
    vec3__normalize((struct Vec3){ 10, 0, 1 })
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

  steam__draw_column(&cam, gpu, norm_steam_light_direction, &steam0);
  steam__draw_column(&cam, gpu, norm_steam_light_direction, &steam1);
  steam__draw_column(&cam, gpu, norm_steam_light_direction, &steam2);

  water__draw(&cam, gpu);

  // debugging__draw_space_gizmo(
  //   gpu,
  //   &cam,
  //   &WORLDSPACE,
  //   ORIGIN
  // );

  gpu->cull_back_faces();
}
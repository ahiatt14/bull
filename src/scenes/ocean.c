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
#include "sky_cylinder_mesh.h"
#include "mountain_mesh.h"
// #include "angel_mesh.h"

#include "steam_texture.h"
#include "mountain_texture.h"
#include "water_texture.h"
#include "clouds_texture.h"
// #include "statue_texture.h"

// #include "statue_frag.h"
#include "mountain_frag.h"
#include "default_vert.h"
#include "water_surface_frag.h"
#include "sky_frag.h"

// CONSTANTS

#define CAMERA_ROTATE_SPEED 0

// READ ONLY

static const struct vec2 WIND_KM_PER_SEC = {
  -0.0075f,
  -0.01f
};

// FORWARD DECS

// LOCALS

static struct vec3 steam_light_direction = {
  1,
  0,
  0
};

// SKY

static struct shader sky_shader;
static struct m4x4 sky_local_to_world;
static struct m3x3 sky_normals_local_to_world;
static struct transform sky_transform = {
  { 0, -1, 0 }, { 0, 180, 0 }, 32
};

// MOUNTAINS

static struct shader mountain_shader;
static struct m4x4 mountain_local_to_world;
static struct m3x3 mountain_normals_local_to_world;
static struct transform mountain_transform = (struct transform){
  .position = { 7, -0.2f, -18 },
  .rotation_in_deg = { 0, 300, 0 },
  .scale = 4
};
static struct m4x4 mountain2_local_to_world;
static struct m3x3 mountain2_normals_local_to_world;
static struct transform mountain2_transform = (struct transform){
  .position = { -11, -0.2f, 8 },
  .rotation_in_deg = { 0, 300, 0 },
  .scale = 4
};

// STATUE

// static struct shader statue_shader;
// static struct m4x4 statue_local_to_world;
// static struct m3x3 statue_normals_local_to_world;
// static struct transform statue_transform = (struct transform){
//   .position = { -7, -3, -25 },
//   .rotation_in_deg = { 10, 65, 30 },
//   .scale = 2
// };

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
// static struct steam_column steam3 = (struct steam_column){
//   .position = { 1, -1.5, 3 },
//   .shape_index_offset = 4
// };

static struct camera cam;

void ocean__init(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu
) {

  cam.position = (struct vec3){ 0, 0.05, 7 };
  cam.look_target = (struct vec3){ 0, 1.05, 0 };
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
  // steam__column_default(&steam3);
  steam__copy_assets_to_gpu(gpu);

  // SKY

  sky_shader.frag_src = sky_frag_src;
  sky_shader.vert_src = default_vert_src;
  gpu->copy_shader_to_gpu(&sky_shader);
  mesh__tile_uvs(3, 3, &sky_cylinder_mesh);
  gpu->copy_static_mesh_to_gpu(&sky_cylinder_mesh);
  gpu->copy_texture_to_gpu(&clouds_texture);
  space__create_model(
    &WORLDSPACE,
    &sky_transform,
    &sky_local_to_world
  );
  space__create_normals_model(
    &sky_local_to_world,
    &sky_normals_local_to_world
  );

  // STATUE

  // gpu->copy_static_mesh_to_gpu(&angel_mesh);
  // statue_shader.frag_src = statue_frag_src;
  // statue_shader.vert_src = default_vert_src;
  // gpu->copy_shader_to_gpu(&statue_shader);
  // space__create_model(
  //   &WORLDSPACE,
  //   &statue_transform,
  //   &statue_local_to_world
  // );
  // space__create_normals_model(
  //   &statue_local_to_world,
  //   &statue_normals_local_to_world
  // );

  // MOUNTAINS

  mountain_shader.frag_src = mountain_frag_src;
  mountain_shader.vert_src = default_vert_src;
  gpu->copy_shader_to_gpu(&mountain_shader);
  mesh__tile_uvs(2, 2, &mountain_mesh);
  gpu->copy_static_mesh_to_gpu(&mountain_mesh);
  gpu->copy_texture_to_gpu(&mountain_texture);
  space__create_model(
    &WORLDSPACE,
    &mountain_transform,
    &mountain_local_to_world
  );
  space__create_normals_model(
    &mountain_local_to_world,
    &mountain_normals_local_to_world
  );
  space__create_model(
    &WORLDSPACE,
    &mountain2_transform,
    &mountain2_local_to_world
  );
  space__create_normals_model(
    &mountain2_local_to_world,
    &mountain2_normals_local_to_world
  );
}

void ocean__tick(
  struct gametime time, 
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu,
  uint8_t previous_scene,
  void (*switch_scene)(uint8_t new_scene)
) {

  // UPDATE

  static struct m4x4 camera_rotation;
  m4x4__rotation(
    deg_to_rad(time.delta * CAMERA_ROTATE_SPEED),
    WORLDSPACE.up,
    &camera_rotation
  );
  cam.position = m4x4_x_point(&camera_rotation, cam.position);
  steam_light_direction = m4x4_x_point(&camera_rotation, steam_light_direction);

  static struct vec3 norm_steam_light_direction;
  norm_steam_light_direction = vec3__normalize(steam_light_direction);

  water__update_waves(WIND_KM_PER_SEC, time, gpu);

  steam__rise(time.delta, &steam0);
  steam__rise(time.delta, &steam1);
  steam__rise(time.delta, &steam2);
  // steam__rise(time.delta, &steam3);
  
  // DRAW

  camera__calculate_lookat(WORLDSPACE.up, &cam);
  camera__calculate_perspective(vwprt, &cam);

  // SKY
  
  // TODO: go invert the sky cylinder normals in blender ya shmole
  gpu->cull_no_faces();

  // TODO: draw wireframe to help debug

  gpu->select_shader(&sky_shader);
  gpu->select_texture(&clouds_texture);
  gpu->set_shader_vec3(&sky_shader, "horizon_color", COLOR_DARK_SLATE_GREY);
  gpu__set_mvp(
    &sky_local_to_world,
    &sky_normals_local_to_world,
    &cam,
    &sky_shader,
    gpu
  );
  gpu->draw_mesh(&sky_cylinder_mesh);

  // STATUE

  gpu->cull_back_faces();

  // gpu->select_shader(&statue_shader);
  // gpu->set_shader_vec3(
  //   &statue_shader,
  //   "light_dir",
  //   vec3__normalize((struct vec3){ -1, -4, 2 })
  // );
  // gpu->set_shader_vec3(
  //   &statue_shader,
  //   "color",
  //   COLOR_DARK_SLATE_GREY
  // );
  // gpu->set_shader_vec3(
  //   &statue_shader,
  //   "light_color",
  //   COLOR_GOLDEN_YELLOW
  // );
  // gpu__set_mvp(
  //   &statue_local_to_world,
  //   &statue_normals_local_to_world,
  //   &cam,
  //   &statue_shader,
  //   gpu
  // );
  // gpu->draw_mesh(&angel_mesh);

  // MOUNTAINS

  gpu->select_shader(&mountain_shader);
  gpu->select_texture(&mountain_texture);
  gpu->set_shader_vec3(
    &mountain_shader,
    "light_dir",
    vec3__normalize((struct vec3){ 10, 0, 1 })
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
  gpu->draw_mesh(&mountain_mesh);
  gpu->set_shader_vec3(
    &mountain_shader,
    "light_dir",
    vec3__normalize((struct vec3){ -1, 0, 1 })
  );
  gpu__set_mvp(
    &mountain2_local_to_world,
    &mountain2_normals_local_to_world,
    &cam,
    &mountain_shader,
    gpu
  );
  gpu->draw_mesh(&mountain_mesh);

  steam__draw_column(&cam, gpu, norm_steam_light_direction, &steam0);
  steam__draw_column(&cam, gpu, norm_steam_light_direction, &steam1);
  steam__draw_column(&cam, gpu, norm_steam_light_direction, &steam2);
  // steam__draw_column(&cam, gpu, norm_steam_light_direction, &steam3);

  water__draw(&cam, gpu);

  // debugging__draw_space_gizmo(
  //   gpu,
  //   &cam,
  //   &WORLDSPACE,
  //   ORIGIN
  // );

  gpu->cull_back_faces();
}
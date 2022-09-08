#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "tail.h"

#include "scene.h"
#include "constants.h"
#include "bull_math.h"
#include "gpu_helpers.h"

#include "water.h"
#include "steam.h"
#include "cage_mesh.h"
#include "mountain_mesh.h"

// #include "cloud_cover_texture.h"
#include "mountain_texture.h"
#include "water_texture.h"
// #include "stars_texture.h"

#include "mountain_frag.h"
#include "default_vert.h"
#include "water_surface_frag.h"
#include "sky_frag.h"

// CONSTANTS

static const struct vec2 WIND_KM_PER_SEC = {
  -0.0075f,
  -0.01f
};

static const struct vec3 SUNLIGHT_DIRECTION = {
  0.92998f,
  0.11624f,
  -0.34874f
};

// FORWARD DECS

// LOCALS

// static struct transform stars_quad_transform = {
//   { -2, 2, 0 }, { 0, 0, 0 }, 1
// };
// static struct m4x4 stars_local_to_world;
// static struct m3x3 stars_normals_local_to_world;

static struct shader sky_shader;
static struct m4x4 sky_local_to_world;
static struct m3x3 sky_normals_local_to_world;
static struct transform sky_transform = {
  { 0, 0, 0 }, { 0, 0, 0 }, 4.5f
};

static struct shader mountain_shader;
static struct m4x4 mountain_local_to_world;
static struct m3x3 mountain_normals_local_to_world;
static struct transform mountain_transform = (struct transform){
  .position = { 13, -0.14f, -18 },
  .rotation_in_deg = { 0, 330, 0 },
  .scale = 5
};

static struct steam_column steam0 = (struct steam_column){
  .position = { 4, -1, -15 },
  .shape_index_offset = 5
};
static struct steam_column steam1 = (struct steam_column){
  .position = { -3, -1, -2 },
  .shape_index_offset = 10
};

static struct camera cam;

void ocean__init(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu
) {

  cam.position = (struct vec3){ 0, 0.15f, 7 };
  cam.look_target = (struct vec3){ 0, 0.2f, 0 };
  cam.horizontal_fov_in_deg = 60;
  cam.near_clip_distance = 0.3f;
  cam.far_clip_distance = 60;
  camera__calculate_lookat(WORLDSPACE.up, &cam);
  camera__calculate_perspective(vwprt, &cam);

  water__init_mesh_data();
  water__copy_assets_to_gpu(gpu);

  steam__init_shared_mesh_data();
  steam__column_default(&steam0);
  steam__column_default(&steam1);
  steam__copy_assets_to_gpu(gpu);

  sky_shader.frag_shader_src = sky_frag_src;
  sky_shader.vert_shader_src = default_vert_src;
  gpu->copy_shader_to_gpu(&sky_shader);
  gpu->copy_static_mesh_to_gpu(&cage_mesh);
  space__create_model(
    &WORLDSPACE,
    &sky_transform,
    &sky_local_to_world
  );
  space__create_normals_model(
    &sky_local_to_world,
    &sky_normals_local_to_world
  );

  // space__create_model(
  //   &WORLDSPACE,
  //   &stars_quad_transform,
  //   &stars_local_to_world
  // );
  // space__create_normals_model(
  //   &stars_local_to_world,
  //   &stars_normals_local_to_world
  // );

  // gpu->copy_texture_to_gpu(&stars_texture);

  mountain_shader.frag_shader_src = mountain_frag_src;
  mountain_shader.vert_shader_src = default_vert_src;
  gpu->copy_shader_to_gpu(&mountain_shader);
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
}

void ocean__tick(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu,
  uint8_t previous_scene,
  void (*switch_scene)(uint8_t new_scene)
) {

  // GAME TIME
  static double seconds_since_creation;
  static double tick_start_time;
  static double delta_time;
  tick_start_time = seconds_since_creation;
  seconds_since_creation = window->get_seconds_since_creation();
  delta_time = seconds_since_creation - tick_start_time;
  if (delta_time > DELTA_TIME_CAP) delta_time = DELTA_TIME_CAP;

  // UPDATE

  static struct m4x4 camera_rotation;
  m4x4__rotation(
    deg_to_rad(delta_time * 3),
    WORLDSPACE.up,
    &camera_rotation
  );
  cam.position = m4x4_x_point(
    &camera_rotation,
    cam.position
  );
  // cam.position.y =
  //   0.1f * sin(seconds_since_creation * 0.02f) + 0.15f;

  water__update_waves(
    WIND_KM_PER_SEC,
    delta_time,
    seconds_since_creation,
    gpu
  );

  steam__rise(delta_time, &steam0);
  steam__rise(delta_time, &steam1);
  
  // DRAW

  camera__calculate_lookat(WORLDSPACE.up, &cam);
  camera__calculate_perspective(vwprt, &cam);

  gpu->cull_back_faces();

  gpu->select_shader(&sky_shader);
  gpu->set_fragment_shader_vec3(
    &sky_shader,
    "night_color",
    COLOR_DARK_SLATE_GREY
  );
  gpu->set_fragment_shader_vec3(
    &sky_shader,
    "light_dir",
    SUNLIGHT_DIRECTION
  );
  gpu->set_fragment_shader_vec3(
    &sky_shader,
    "light_color",
    COLOR_AQUA_BLUE
  );
  gpu__set_mvp(
    &sky_local_to_world,
    &sky_normals_local_to_world,
    &cam,
    &sky_shader,
    gpu
  );
  gpu->draw_mesh(&cage_mesh);

  // gpu->select_shader(&ALPHA_TEXTURE_SHADER);
  // gpu->select_texture(&stars_texture);
  // gpu__set_mvp(
  //   &stars_local_to_world,
  //   &stars_normals_local_to_world,
  //   &cam,
  //   &ALPHA_TEXTURE_SHADER,
  //   gpu
  // );
  // gpu->draw_mesh(&QUAD);

  gpu->clear_depth_buffer();

  gpu->select_shader(&mountain_shader);
  gpu->select_texture(&mountain_texture);
  gpu->set_fragment_shader_vec3(
    &mountain_shader,
    "light_dir",
    SUNLIGHT_DIRECTION
  );
  gpu->set_fragment_shader_vec3(
    &mountain_shader,
    "light_color",
    COLOR_NEON_PURPLE
  );
  gpu__set_mvp(
    &mountain_local_to_world,
    &mountain_normals_local_to_world,
    &cam,
    &mountain_shader,
    gpu
  );
  gpu->draw_mesh(&mountain_mesh);

  steam__draw_column(&cam, gpu, SUNLIGHT_DIRECTION, &steam0);
  steam__draw_column(&cam, gpu, SUNLIGHT_DIRECTION, &steam1);

  water__draw(&cam, gpu);

  gpu->cull_back_faces();
}
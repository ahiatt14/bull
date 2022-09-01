#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "tail.h"

#include "scene.h"
#include "constants.h"
#include "bull_math.h"
#include "gpu_helpers.h"

#include "water.h"
#include "turbine.h"
#include "steam.h"
#include "cage_mesh.h"
#include "mountain_mesh.h"

#include "clouds_texture.h"
#include "cloud_cover_texture.h"
#include "water_texture.h"
#include "stars_texture.h"

#include "turbine_frag.h"
#include "default_vert.h"
#include "water_surface_frag.h"
#include "sky_frag.h"

// CONSTANTS

#define TURBINE_X_COUNT 5
#define TURBINE_Z_COUNT 5
#define TURBINE_X_OFFSET 3.0f
#define TURBINE_Z_OFFSET 2.0f
#define TURBINE_FIELD_KM_WIDE (TURBINE_X_COUNT - 1) * TURBINE_X_OFFSET
#define TURBINE_FILE_KM_DEEP (TURBINE_Z_COUNT - 1) * TURBINE_Z_OFFSET

static const struct vec2 TURBINE_FIELD_ORIGIN_OFFSET = {
  -TURBINE_FIELD_KM_WIDE * 0.5f,
  -TURBINE_FILE_KM_DEEP * 0.5f
};

static const struct vec2 WIND_KM_PER_SEC = {
  -0.0075f,
  -0.005f
};

static const struct vec3 SUNLIGHT_DIRECTION = {
  0.707f,
  -0.707f,
  0
};

// FORWARD DECS

static struct vec2 relative_position_within_ocean(struct vec3 pos) {
  return (struct vec2){
    (pos.x + OCEAN_KM_WIDE * 0.5f) / OCEAN_KM_WIDE,
    (pos.z + OCEAN_KM_WIDE * 0.5f) / OCEAN_KM_WIDE
  };
}
static float brightness(float r, float g, float b) {
  return (r + g + b) / (255.0f * 3.0f);
}

// LOCALS

static struct transform stars_quad_transform = {
  { -2, 2, 0 }, { 0, 0, 0 }, 1
};
static struct m4x4 stars_local_to_world;
static struct m3x3 stars_normals_local_to_world;

static struct shader sky_shader;
static struct m4x4 sky_local_to_world;
static struct m3x3 sky_normals_local_to_world;
static struct transform sky_transform = {
  { 0, 0, 0 }, { 0, 0, 0 }, 4.5f
};

static struct turbine turbines[TURBINE_X_COUNT * TURBINE_Z_COUNT];

static struct shader mountain_shader;
static struct m4x4 mountain_local_to_world;
static struct m3x3 mountain_normals_local_to_world;
static struct transform mountain_transform = (struct transform){
  .position = { 14, -0.12f, -20 },
  .rotation_in_deg = { 0, 330, 0 },
  .scale = 5
};

static struct steam_column steam0 = (struct steam_column){
  .position = { 6, -1, -18 },
  .shape_index_offset = 5
};
static struct steam_column steam1 = (struct steam_column){
  .position = { -6, -1, -13 },
  .shape_index_offset = 1
};

static struct camera cam;

void ocean__init(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu
) {

  cam.position = (struct vec3){ -1.2f, 0.15f, 7 };
  cam.look_target = (struct vec3){ 0, 0.2f, 0 };
  cam.horizontal_fov_in_deg = 60;
  cam.near_clip_distance = 0.3f;
  cam.far_clip_distance = 60;
  camera__calculate_lookat(WORLDSPACE.up, &cam);
  camera__calculate_perspective(vwprt, &cam);

  water__init_mesh_data();
  water__copy_assets_to_gpu(gpu);

  steam__shared_init_mesh_data();
  steam__column_default(WIND_KM_PER_SEC, &steam0);
  steam__column_default(WIND_KM_PER_SEC, &steam1);
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

  space__create_model(
    &WORLDSPACE,
    &stars_quad_transform,
    &stars_local_to_world
  );
  space__create_normals_model(
    &stars_local_to_world,
    &stars_normals_local_to_world
  );

  mountain_shader.frag_shader_src = turbine_frag_src;
  mountain_shader.vert_shader_src = default_vert_src;
  gpu->copy_shader_to_gpu(&mountain_shader);
  gpu->copy_static_mesh_to_gpu(&mountain_mesh);
  space__create_model(
    &WORLDSPACE,
    &mountain_transform,
    &mountain_local_to_world
  );
  space__create_normals_model(
    &mountain_local_to_world,
    &mountain_normals_local_to_world
  );

  turbine__copy_assets_to_gpu(gpu);
  for (int x = 0; x < TURBINE_X_COUNT; x++)
  for (int z = 0; z < TURBINE_Z_COUNT; z++)
    turbines[x + z * TURBINE_X_COUNT] = (struct turbine){
      .rotation_deg_per_sec = 20 + (float)(rand() / (float)RAND_MAX) * 70,
      .transform = {
        {
          x * TURBINE_X_OFFSET + TURBINE_FIELD_ORIGIN_OFFSET.x + x * x,
          0,
          z * TURBINE_Z_OFFSET + TURBINE_FIELD_ORIGIN_OFFSET.y
        },
        { 0, 0, 0 },
        0.12f
      }
    };
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

  // static struct m4x4 camera_rotation;
  // m4x4__rotation(
  //   deg_to_rad(delta_time * 3),
  //   WORLDSPACE.up,
  //   &camera_rotation
  // );
  // cam.position = m4x4_x_point(
  //   &camera_rotation,
  //   cam.position
  // );
  // cam.position.y =
  //   0.1f * sin(seconds_since_creation * 0.02f) + 0.15f;
  // camera__calculate_lookat(WORLDSPACE.up, &cam);
  // camera__calculate_perspective(vwprt, &cam);

  static struct vec2 clouds_offset;
  clouds_offset.x = loop_float(
    clouds_offset.x + WIND_KM_PER_SEC.x * delta_time, 0, 1
  );
  clouds_offset.y = loop_float(
    clouds_offset.y + WIND_KM_PER_SEC.y * delta_time, 0, 1
  );

  water__update_waves(
    WIND_KM_PER_SEC,
    delta_time,
    seconds_since_creation,
    gpu
  );

  steam__rise(delta_time, &steam0);
  steam__rise(delta_time, &steam1);

  static struct vec2 relative_turbine_position;
  static struct vec2 normalized_sample_xy;
  static int_fast32_t sample_pixel_x = 0;
  static int_fast32_t sample_pixel_y = 0;
  static int_fast32_t sample_index = 0;
  for (int i = 0; i < TURBINE_X_COUNT * TURBINE_Z_COUNT; i++) {

    turbines[i].blades_rotation_in_deg -=
      turbines[i].rotation_deg_per_sec * delta_time;

    relative_turbine_position =
      relative_position_within_ocean(turbines[i].transform.position);
    normalized_sample_xy.x =
      loop_float(relative_turbine_position.x + 1 - clouds_offset.x, 0, 1);
    normalized_sample_xy.y =
      loop_float(relative_turbine_position.y + 1 - clouds_offset.y, 0, 1);
    sample_pixel_x =
      normalized_sample_xy.x * (cloud_cover_texture.width - 1);
    sample_pixel_y =
      normalized_sample_xy.y * (cloud_cover_texture.height - 1);
    sample_index =
      (sample_pixel_x + sample_pixel_y * cloud_cover_texture.width) *
      cloud_cover_texture.channels_count;
    turbines[i].ratio_of_sunlight = 1 - brightness(
      cloud_cover_texture.data[sample_index],
      cloud_cover_texture.data[sample_index+1],
      cloud_cover_texture.data[sample_index+2]
    );
  }
  
  // DRAW

  // camera__calculate_lookat(WORLDSPACE.up, &cam);
  // camera__calculate_perspective(vwprt, &cam);

  gpu->cull_back_faces();

  gpu->select_shader(&sky_shader);
  gpu->select_texture(&stars_texture);
  gpu->set_fragment_shader_vec3(
    &sky_shader,
    "night_color",
    COLOR_LIGHT_GREY
  );
  gpu->set_fragment_shader_vec3(
    &sky_shader,
    "light_dir",
    vec3__negate(SUNLIGHT_DIRECTION)
  );
  gpu->set_fragment_shader_vec3(
    &sky_shader,
    "light_color",
    COLOR_WHITE
  );
  gpu__set_mvp(
    &sky_local_to_world,
    &sky_normals_local_to_world,
    &cam,
    &sky_shader,
    gpu
  );
  gpu->draw_mesh(&cage_mesh);

  gpu->select_shader(&ALPHA_TEXTURE_SHADER);
  gpu->select_texture(&stars_texture);
  gpu__set_mvp(
    &stars_local_to_world,
    &stars_normals_local_to_world,
    &cam,
    &ALPHA_TEXTURE_SHADER,
    gpu
  );
  gpu->draw_mesh(&QUAD);

  gpu->clear_depth_buffer();

  gpu->select_shader(&mountain_shader);
  gpu->select_texture(&clouds_texture);
  gpu->set_fragment_shader_vec3(
    &mountain_shader,
    "light_dir",
    (struct vec3){ 1, 0, 0 }
  );
  gpu->set_fragment_shader_vec3(
    &mountain_shader,
    "light_color",
    COLOR_GOLDEN_YELLOW
  );
  gpu->set_fragment_shader_vec3(
    &mountain_shader,
    "color",
    COLOR_DEEP_FOREST_GREEN
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

  water__draw(
    SUNLIGHT_DIRECTION,
    COLOR_EVENING_SUNLIGHT,
    &cam,
    gpu
  );

  for (int i = 0; i < TURBINE_X_COUNT * TURBINE_Z_COUNT; i++)
    turbine__draw(
      &cam,
      gpu,
      SUNLIGHT_DIRECTION,
      COLOR_EVENING_SUNLIGHT,
      &turbines[i]
    );

  gpu->cull_back_faces();
}
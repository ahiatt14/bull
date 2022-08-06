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
#include "quad.h"
#include "cage_mesh.h"

#include "clouds_texture.h"
#include "cloud_cover_texture.h"
#include "water_texture.h"

#include "arena_cage_frag.h"
#include "solid_color_frag.h"
#include "flat_texture_frag.h"
#include "default_vert.h"
#include "water_surface_frag.h"
#include "normal_debug_frag.h"
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
  0,
  0.01f
};

static const struct vec3 SUNLIGHT_DIRECTION = {
  0.707f,
  -0.707f,
  0
};

// LOCALS

static struct shader sky_shader;
static struct transform sky_transform = {
  {0, 0, 0}, {0, 0, 0}, 4.5f
};

static struct camera foreground_camera;

static struct vec2 relative_position_within_ocean(struct vec3 pos) {
  return (struct vec2){
    (pos.x + OCEAN_KM_WIDE * 0.5f) / OCEAN_KM_WIDE,
    (pos.z + OCEAN_KM_WIDE * 0.5f) / OCEAN_KM_WIDE
  };
}
static float brightness(float r, float g, float b) {
  return (r + g + b) / (255.0f * 3.0f);
}

// TURBINES

static struct turbine turbines[TURBINE_X_COUNT * TURBINE_Z_COUNT];

void ocean__init(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu
) {

  foreground_camera.position = (struct vec3){ -1.2f, 0.2f, 7 };
  foreground_camera.look_target = (struct vec3){ 0, 0.1f, 0 };
  foreground_camera.horizontal_fov_in_deg = 60;
  foreground_camera.near_clip_distance = 0.3f;
  foreground_camera.far_clip_distance = 13;

  water__init_mesh_data();
  water__copy_assets_to_gpu(gpu);

  sky_shader.frag_shader_src = sky_frag_src;
  sky_shader.vert_shader_src = default_vert_src;
  gpu->copy_shader_to_gpu(&sky_shader);
  gpu->copy_static_mesh_to_gpu(&cage_mesh);

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

  static struct m4x4 camera_rotation;
  // TODO: if ur gonna be too lazy to add quaternion rotations,
  // at least make a fn for rotating a point
  m4x4__rotation(
    deg_to_rad(delta_time * 1),
    WORLDSPACE.up,
    &camera_rotation
  );
  foreground_camera.position = m4x4_x_point(
    &camera_rotation,
    foreground_camera.position
  );
  foreground_camera.position.y =
    0.1f * sin(seconds_since_creation * 0.02f) + 0.15f;

  sky_transform.rotation_in_deg.x += 30 * delta_time;

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

  camera__calculate_lookat(WORLDSPACE.up, &foreground_camera);
  camera__calculate_perspective(vwprt, &foreground_camera);

  gpu->cull_back_faces();

  static struct m4x4 sky_local_to_world;
  static struct m3x3 sky_normals_local_to_world;
  gpu->select_shader(&sky_shader);
  gpu->set_fragment_shader_vec3(
    &sky_shader,
    "night_color",
    COLOR_AQUA_BLUE
  );
  space__create_model(
    &WORLDSPACE,
    &sky_transform,
    &sky_local_to_world
  );
  space__create_normals_model(
    &sky_local_to_world,
    &sky_normals_local_to_world
  );
  gpu__set_mvp(
    &sky_local_to_world,
    &sky_normals_local_to_world,
    &foreground_camera,
    &sky_shader,
    gpu
  );
  gpu->draw_mesh(&cage_mesh);

  gpu->clear_depth_buffer();

  water__draw(
    SUNLIGHT_DIRECTION,
    COLOR_EVENING_SUNLIGHT,
    &foreground_camera,
    gpu
  );

  for (int i = 0; i < TURBINE_X_COUNT * TURBINE_Z_COUNT; i++)
    turbine__draw(
      &foreground_camera,
      gpu,
      SUNLIGHT_DIRECTION,
      COLOR_EVENING_SUNLIGHT,
      &turbines[i]
    );

  gpu->cull_back_faces();
}
#include <stdio.h>
#include <math.h>

#include "tail.h"

#include "scene.h"
#include "constants.h"
#include "bull_math.h"
#include "gpu_helpers.h"

#include "water.h"
#include "turbine.h"

#include "cloud_cover_texture.h"
#include "water_texture.h"

#include "default_vert.h"
#include "water_surface_frag.h"
#include "normal_debug_frag.h"

// CONSTANTS

#define CLOUD_KM_PER_SECOND 0.08f

#define TURBINE_COUNT 16
#define TURBINE_ALTITUDE 0
#define TURBINE_X_OFFSET 1.5f
#define TURBINE_Z_OFFSET 0.8f

static const struct vec3 SUNLIGHT_DIRECTION = {
  0.707,
  -0.707,
  0
};

static const struct vec3 TURBINE_FIELD_ORIGIN_OFFSET = {
  -sqrt(TURBINE_COUNT) * 0.5f * TURBINE_X_OFFSET,
  0,
  -sqrt(TURBINE_COUNT) * 0.5f * TURBINE_Z_OFFSET
};

// LOCALS

static double seconds_since_creation;
static double tick_start_time;
static double delta_time;

static struct camera cam;

static struct vec2 ocean_uv_scroll_acc;

// TURBINES

static struct turbine turbines[TURBINE_COUNT];

void ocean__init(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu
) {

  camera__init(&cam);
  camera__set_position(0, 0.07f, 2, &cam);
  camera__set_look_target((struct vec3){ 0, 0.3f, 0 }, &cam);
  camera__set_horizontal_fov_in_deg(60, &cam);
  camera__set_near_clip_distance(0.3f, &cam);
  camera__set_far_clip_distance(16, &cam);
  camera__calculate_lookat(WORLDSPACE.up, &cam);
  camera__calculate_perspective(vwprt, &cam);

  water__init_mesh_data();
  water__copy_assets_to_gpu(gpu);

  // gpu->copy_rgb_texture_to_gpu(&clod256_texture);

  turbine__copy_assets_to_gpu(gpu);
  turbines[0] = (struct turbine){
    {{ 0, 0, 0 },{ 0, 0, 0 },0.1f}
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

  tick_start_time = seconds_since_creation;
  seconds_since_creation = window->get_seconds_since_creation();
  delta_time = seconds_since_creation - tick_start_time;
  if (delta_time > DELTA_TIME_CAP) delta_time = DELTA_TIME_CAP;

  // UPDATE

  water__update_waves(
    CLOUD_KM_PER_SECOND,
    delta_time,
    seconds_since_creation,
    gpu
  );
  
  // DRAW

  gpu->clear(&COLOR_SKY_BLUE);

  water__draw(
    SUNLIGHT_DIRECTION,
    COLOR_EVENING_SUNLIGHT,
    &cam,
    gpu
  );

  // TODO: cache these maybe
  struct turbine *current_turb = NULL;
  struct vec2 shadow_uv = (struct vec2){0};
  for (int i = 0; i < TURBINE_COUNT; i++) {
    current_turb = &turbines[i];
    turbine__spin_blades(
      delta_time,
      -30,
      current_turb
    );
    // shadow_uv = (struct vec2){
    //   current_turb->transform.position.x / OCEAN_WIDTH + ocean_uv_scroll_acc.x,
    //   current_turb->transform.position.z / OCEAN_WIDTH + ocean_uv_scroll_acc.y
    // };
    turbine__draw(
      &cam,
      gpu,
      // shadow_uv,
      SUNLIGHT_DIRECTION,
      COLOR_EVENING_SUNLIGHT,
      current_turb
    );
  }

  gpu->cull_back_faces();
}
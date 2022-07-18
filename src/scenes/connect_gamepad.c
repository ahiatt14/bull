#include <stdint.h>

#include "tail.h"

#include "scene.h"
#include "constants.h"
#include "gpu_helpers.h"

#include "exclamation_mesh.h"

#include "solid_color_frag.h"
#include "connect_gamepad_bg_frag.h"
#include "default_vert.h"

#define EXCLAIM_HORIZONTAL_SPACING 0.2f
#define EXCLAIM_VERTICAL_SPACING 0.5f
#define EXPLAIN_SCROLL_SPEED 2

static struct camera cam;

static double seconds_since_creation;
static double tick_start_time;
static double delta_time;

static struct transform exclamation_transform = {{0,0,0},{90,0,0},1};
static struct shader exclamation_shader;
static struct m4x4 exclamation_local_to_world;

void connect_gamepad__init(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu
) {
  gpu->cull_no_faces();

  camera__init(&cam);
  camera__set_position(0, 0, 3, &cam);
  camera__set_look_target(&ORIGIN, &cam);
  camera__set_horizontal_fov_in_deg(80, &cam);
  camera__set_near_clip_distance(0.1f, &cam);
  camera__set_far_clip_distance(100, &cam);
  camera__calculate_lookat(&WORLDSPACE.up, &cam);
  camera__calculate_perspective(vwprt, &cam);

  // exclamation_shader.frag_shader_src = connect_gamepad_bg_frag_src;
  exclamation_shader.frag_shader_src = solid_color_frag_src;
  exclamation_shader.vert_shader_src = default_vert_src;
  gpu->copy_shader_to_gpu(&exclamation_shader);

  gpu->copy_static_mesh_to_gpu(&exclamation_mesh);

  // struct drawable_mesh batched_exclamation = {
  //   .vertices = 
  // };
}

void connect_gamepad__tick(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu,
  uint8_t previous_scene,
  void (*switch_scene)(uint8_t new_scene)
) {
  tick_start_time = seconds_since_creation;
  seconds_since_creation = window->get_seconds_since_creation();
  delta_time = seconds_since_creation - tick_start_time;
  if (delta_time > DELTA_TIME_CAP) delta_time = DELTA_TIME_CAP;

  // UPDATE
  if (window->gamepad_is_connected()) {
    switch_scene(previous_scene);
    return;
  }

  // DRAW
  gpu->clear(&COLOR_BLACK);

  gpu->select_shader(&exclamation_shader);
  space__create_model(
    &WORLDSPACE,
    &exclamation_transform,
    &exclamation_local_to_world
  );
  gpu__set_mvp(
    &exclamation_local_to_world,
    &(struct m3x3){{1,0,0,0,1,0,0,0,1}},
    &cam,
    &exclamation_shader,
    gpu
  );
  gpu->set_fragment_shader_vec3(
    &exclamation_shader,
    "color",
    &COLOR_RED
  );
  gpu->draw_mesh(&exclamation_mesh);
}
#include <math.h>
#include <stdint.h>

#include "tail.h"

#include "scene.h"
#include "constants.h"
#include "tail_helpers.h"

#include "exclamation_mesh.h"

#include "default_vert.h"

#define EXCLAIM_HORIZONTAL_SPACING 0.2f
#define EXCLAIM_VERTICAL_SPACING 0.5f
#define EXPLAIN_SCROLL_SPEED 2

static struct camera cam;

static struct transform exclamation_transform;
static struct m4x4 exclamation_local_to_world;

void connect_gamepad__init(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu
) {
  gpu->cull_no_faces();

  exclamation_transform = (struct transform){
    ORIGIN,
    quaternion__create(WORLDSPACE.right, M_PI * 0.5f), 
    1
  };

  cam.position = (struct vec3){ 0, 0, 3 };
  cam.look_target = ORIGIN;
  cam.horizontal_fov_in_deg = 80;
  cam.near_clip_distance = 0.1f;
  cam.far_clip_distance = 100;
  camera__calculate_lookat(WORLDSPACE.up, &cam);
  camera__calculate_perspective(vwprt, &cam);

  gpu->copy_static_mesh_to_gpu(&exclamation_mesh);

  // struct drawable_mesh batched_exclamation = {
  //   .vertices = 
  // };
}

void connect_gamepad__tick(
  struct GameTime time,
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu,
  uint8_t previous_scene,
  void (*switch_scene)(uint8_t new_scene)
) {

  // UPDATE
  if (window->gamepad_is_connected()) {
    switch_scene(previous_scene);
    return;
  }

  // DRAW
  gpu->clear(&COLOR_BLACK);

  gpu->select_shader(&SOLID_COLOR_SHADER);
  space__create_model(
    &WORLDSPACE,
    &exclamation_transform,
    &exclamation_local_to_world
  );
  gpu__set_mvp(
    &exclamation_local_to_world,
    &M3X3_IDENTITY,
    &cam,
    &SOLID_COLOR_SHADER,
    gpu
  );
  gpu->set_shader_vec3(
    &SOLID_COLOR_SHADER,
    "color",
    COLOR_RED
  );
  gpu->draw_mesh(&exclamation_mesh);
}
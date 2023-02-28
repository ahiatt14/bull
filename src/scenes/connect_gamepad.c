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

static Camera cam;

static Transform exclamation_transform;
static M4x4 exclamation_local_to_world;

void connect_gamepad__init(
  Window const *const window,
  Viewport *const vwprt,
  GPU const *const gpu
) {
  gpu->cull_no_faces();

  exclamation_transform = (Transform){
    ORIGIN,
    quaternion__create(WORLDSPACE.right, M_PI * 0.5f), 
    1
  };

  cam.position = (Vec3){ 0, 0, 3 };
  cam.look_target = ORIGIN;
  cam.horizontal_fov_in_deg = 80;
  cam.near_clip_distance = 0.1f;
  cam.far_clip_distance = 100;
  camera__calculate_lookat(WORLDSPACE.up, &cam);
  camera__calculate_perspective(vwprt, &cam);

  gpu->copy_static_mesh_to_gpu(&EXCLAMATION_MESH);

  // DrawableMesh batched_exclamation = {
  //   .vertices = 
  // };
}

void connect_gamepad__tick(
  GameTime time,
  Window const *const window,
  Viewport *const vwprt,
  GPU const *const gpu,
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
  gpu->draw_mesh(&EXCLAMATION_MESH);
}
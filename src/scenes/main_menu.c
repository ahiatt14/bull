#include <stdio.h>

#include "tail.h"

#include "gpu_helpers.h"
#include "scene.h"
#include "constants.h"

// #include "menu_sky.h"

#include "square.h"
#include "flat_texture_frag.h"
#include "clod256_texture.h"

#include "pyramid_mesh.h"
#include "default_vert.h"
#include "solid_color_frag.h"
#include "normal_debug_frag.h"

#define SEC_UNTIL_ACTION 1.5f

static double seconds_since_creation;
static double tick_start_time;
static double delta_time;

static struct camera foreground_camera;
static struct camera background_camera;

static struct transform pyramid_transform = {{ 0, 0, 0 }, { 0, 0, 0 }, 1};
static struct shader pyramid_shader;
static struct m4x4 pyramid_local_to_world;
static struct m3x3 pyramid_normals_local_to_world;

static double sec_until_action = SEC_UNTIL_ACTION; 
static double seconds_since_creation;

void main_menu__init(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu
) {

  gpu->cull_no_faces();

  camera__init(&foreground_camera);
  camera__init(&background_camera);

  camera__set_position(0, 1.5f, 2, &foreground_camera);
  camera__set_look_target(&ORIGIN, &foreground_camera);
  camera__set_horizontal_fov_in_deg(80, &foreground_camera);
  camera__set_near_clip_distance(0.1f, &foreground_camera);
  camera__set_far_clip_distance(100, &foreground_camera);
  camera__calculate_lookat(&WORLDSPACE.up, &foreground_camera);
  camera__calculate_perspective(vwprt, &foreground_camera);

  camera__set_position(0, 1, 1, &background_camera);
  struct vec3 background_cam_look_target = {0};
  vec3_plus_vec3(
    camera__get_position(&background_camera),
    &(struct vec3){ 0, 0, -1 },
    &background_cam_look_target
  );
  camera__set_look_target(&background_cam_look_target, &background_camera);
  camera__set_horizontal_fov_in_deg(120, &background_camera);
  camera__set_near_clip_distance(0.2f, &background_camera);
  camera__set_far_clip_distance(20, &background_camera);
  camera__calculate_lookat(&WORLDSPACE.up, &background_camera);
  camera__calculate_perspective(vwprt, &background_camera);

  pyramid_shader.frag_shader_src = flat_texture_frag_src;
  pyramid_shader.vert_shader_src = default_vert_src;
  gpu->copy_shader_to_gpu(&pyramid_shader);
  gpu->copy_static_mesh_to_gpu(&pyramid_mesh);

  gpu->copy_rgb_texture_to_gpu(&clod256_texture);

  // menu_sky__init(gpu);
}

void main_menu__tick(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu,
  uint8_t previous_scene,
  void (*switch_scene)(uint8_t new_scene)
) {
  seconds_since_creation = window->get_seconds_since_creation();
  delta_time = seconds_since_creation - tick_start_time;
  if (delta_time > DELTA_TIME_CAP) delta_time = DELTA_TIME_CAP;
  tick_start_time = seconds_since_creation;

  if (!window->gamepad_is_connected()) {
    switch_scene(SCENE__CONNECT_GAMEPAD);
    return;
  }

  // UPDATE

  pyramid_transform.rotation_in_deg.y += 20.0f * delta_time;
  pyramid_transform.position.x += 0.1f * delta_time;
  sec_until_action -= delta_time;
  if (sec_until_action <= 0) {
    sec_until_action = SEC_UNTIL_ACTION;
    pyramid_transform = (struct transform){0};
    switch_scene(SCENE__ACTION);
    return;
  }

  // menu_sky__tick(delta_time, sec onds_since_creation, gpu);

  // DRAW
  gpu->clear(&COLOR_LIGHT_GREY);

  // menu_sky__draw(
  //   gpu,
  //   &background_camera
  // );
  // gpu->clear_depth_buffer();

  gpu->select_shader(&pyramid_shader);
  gpu->select_texture(&clod256_texture);
  space__create_model(
    &WORLDSPACE,
    &pyramid_transform,
    &pyramid_local_to_world
  );
  space__create_normals_model(
    &pyramid_local_to_world,
    &pyramid_normals_local_to_world
  );
  gpu__set_mvp(
    &pyramid_local_to_world,
    &pyramid_normals_local_to_world,
    &foreground_camera,
    &pyramid_shader,
    gpu
  );
  gpu->draw_mesh(&pyramid_mesh);
}
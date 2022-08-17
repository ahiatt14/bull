#include <stdio.h>

#include "tail.h"

#include "gpu_helpers.h"
#include "scene.h"
#include "constants.h"

// #include "menu_sky.h"

#include "core.h"

#include "clouds_texture.h"
#include "pyramid_mesh.h"
#include "smooth_cube_mesh.h"

#include "flat_texture_frag.h"
#include "default_vert.h"
#include "solid_color_frag.h"
#include "normal_debug_frag.h"
#include "normal_debug_geo.h"

#define SEC_UNTIL_ACTION 3000.0f

// static struct core_state core = (struct core_state){
//   {{0, 0, 0}, {0, 0, 0}, 1}
// };

static struct camera foreground_camera;
static struct camera background_camera;

static struct transform pyramid_transform = {{ 1, 0, 0 }, { 0, 0, 0 }, 1};
static struct shader normal_debug_shader;

// TODO: just testing normal debugging geometry shader
static struct shader normal_only_vis_shader;
static struct transform smooth_cube_transform = {{ 0, 1, 0 }, { 0, 0, 0 }, 1};

void main_menu__init(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu
) {

  gpu->cull_no_faces();

  foreground_camera.position = (struct vec3){ 0, 2, 3 };
  foreground_camera.look_target = ORIGIN;
  foreground_camera.horizontal_fov_in_deg = 80;
  foreground_camera.near_clip_distance = 0.1f;
  foreground_camera.far_clip_distance = 100;

  background_camera.position = (struct vec3){ 0, 1, 1 };
  background_camera.look_target = vec3_plus_vec3(
    background_camera.position,
    (struct vec3){ 0, 0, -1 }
  );
  background_camera.horizontal_fov_in_deg = 120;
  background_camera.near_clip_distance = 0.2f;
  background_camera.far_clip_distance = 20;
  camera__calculate_lookat(WORLDSPACE.up, &background_camera);
  camera__calculate_perspective(vwprt, &background_camera);

  core__copy_assets_to_gpu(gpu);

  normal_debug_shader.frag_shader_src = normal_debug_frag_src;
  normal_debug_shader.vert_shader_src = default_vert_src;
  gpu->copy_shader_to_gpu(&normal_debug_shader);

  // TODO: normal debugging
  normal_only_vis_shader.frag_shader_src = solid_color_frag_src;
  normal_only_vis_shader.vert_shader_src = default_vert_src;
  normal_only_vis_shader.geo_shader_src = normal_debug_geo_src;
  gpu->copy_shader_to_gpu(&normal_only_vis_shader);

  gpu->copy_rgb_texture_to_gpu(&clouds_texture);
  gpu->copy_static_mesh_to_gpu(&pyramid_mesh);

  gpu->copy_static_mesh_to_gpu(&smooth_cube_mesh);

  // menu_sky__init(gpu);
}

void main_menu__tick(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu,
  uint8_t previous_scene,
  void (*switch_scene)(uint8_t new_scene)
) {
  static double seconds_since_creation;
  static double tick_start_time;
  static double delta_time;

  tick_start_time = seconds_since_creation;
  seconds_since_creation = window->get_seconds_since_creation();
  delta_time = seconds_since_creation - tick_start_time;
  if (delta_time > DELTA_TIME_CAP) delta_time = DELTA_TIME_CAP;

  if (!window->gamepad_is_connected()) {
    switch_scene(SCENE__CONNECT_GAMEPAD);
    return;
  }

  // UPDATE

  // pyramid_transform.rotation_in_deg.y += 20.0f * delta_time;

  static struct m4x4 cam_rotation;
  m4x4__rotation(
    deg_to_rad(delta_time * 10),
    WORLDSPACE.up,
    &cam_rotation
  );
  foreground_camera.position = m4x4_x_point(
    &cam_rotation,
    foreground_camera.position
  );
  camera__calculate_lookat(WORLDSPACE.up, &foreground_camera);
  camera__calculate_perspective(vwprt, &foreground_camera);

  static double sec_until_action = SEC_UNTIL_ACTION;
  sec_until_action -= delta_time;
  if (sec_until_action <= 0) {
    sec_until_action = SEC_UNTIL_ACTION;
    pyramid_transform = (struct transform){0};
    smooth_cube_transform = (struct transform){0};
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

  static struct m4x4 shared_local_to_world;
  static struct m3x3 shared_normals_local_to_world;

  gpu->select_shader(&normal_debug_shader);
  space__create_model(
    &WORLDSPACE,
    &pyramid_transform,
    &shared_local_to_world
  );
  space__create_normals_model(
    &shared_local_to_world,
    &shared_normals_local_to_world
  );
  gpu__set_mvp(
    &shared_local_to_world,
    &shared_normals_local_to_world,
    &foreground_camera,
    &normal_debug_shader,
    gpu
  );
  gpu->draw_mesh(&pyramid_mesh);

  gpu->select_shader(&normal_only_vis_shader);
  gpu__set_mvp(
    &shared_local_to_world,
    &shared_normals_local_to_world,
    &foreground_camera,
    &normal_only_vis_shader,
    gpu
  );
  gpu->draw_mesh(&pyramid_mesh);

  gpu->select_shader(&normal_debug_shader);
  space__create_model(
    &WORLDSPACE,
    &smooth_cube_transform,
    &shared_local_to_world
  );
  space__create_normals_model(
    &shared_local_to_world,
    &shared_normals_local_to_world
  );
  gpu__set_mvp(
    &shared_local_to_world,
    &shared_normals_local_to_world,
    &foreground_camera,
    &normal_debug_shader,
    gpu
  );
  gpu->draw_mesh(&smooth_cube_mesh);

  gpu->select_shader(&normal_only_vis_shader);
  gpu__set_mvp(
    &shared_local_to_world,
    &shared_normals_local_to_world,
    &foreground_camera,
    &normal_only_vis_shader,
    gpu
  );
  gpu->draw_mesh(&smooth_cube_mesh);

  // core__draw(
  //   &foreground_camera,
  //   gpu,
  //   &core
  // );
}
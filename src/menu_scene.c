// TODO: remove
#include <stdio.h>

#include "tail.h"

#include "gpu_helpers.h"
#include "scene.h"
#include "menu_scene.h"
#include "constants.h"

// #include "menu_sky.h"

#include "square.h"
#include "flat_texture_frag.h"
#include "clod256_texture.h"

#include "pyramid_mesh.h"
#include "default_vert.h"
#include "solid_color_frag.h"
#include "normal_debug_frag.h"

#define DELTA_TIME_CAP 1.0f / 30.0f

double delta_time;
double start_time;

struct camera foreground_camera;
struct camera background_camera;

struct transform pyramid_transform = {{ 0, 0, 0 }, { 0, 0, 0 }, 1};
struct gpu_program pyramid_shader;
struct m4x4 pyramid_local_to_world;
struct m3x3 pyramid_normals_local_to_world;

struct square sky_square;

void menu__init(
  struct window_api *window,
  struct viewport *vwprt,
  struct gpu_api *gpu
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

  square__create(
    (struct transform){{ 0, 0, 0 }, { 0, 0, 0 }, 1},
    clod256_texture,
    normal_debug_frag_src,
    &sky_square,
    gpu
  );

  pyramid_shader.frag_shader_src = flat_texture_frag_src;
  pyramid_shader.vert_shader_src = default_vert_src;
  gpu->copy_program_to_gpu(&pyramid_shader);
  gpu->copy_static_mesh_to_gpu(&pyramid_mesh);

  gpu->copy_rgb_texture_to_gpu(&clod256_texture);

  // menu_sky__init(gpu);
}

void menu__tick(
  double seconds_since_creation,
  const struct viewport *vwprt,
  struct gpu_api *gpu,
  struct scene **scenes,
  void switch_scene(struct scene* new_scene)
) {
  delta_time = seconds_since_creation - start_time;
  if (delta_time > DELTA_TIME_CAP) delta_time = DELTA_TIME_CAP;
  start_time = seconds_since_creation;

  // UPDATE

  pyramid_transform.rotation_in_deg.y += 20.0f * delta_time;

  // menu_sky__tick(delta_time, seconds_since_creation, gpu);

  // DRAW
  // TODO: update clear to accept vec3*
  gpu->clear(&COLOR_LIGHT_GREY);

  // menu_sky__draw(
  //   gpu,
  //   &background_camera
  // );
  // gpu->clear_depth_buffer();

  square__draw(
    &sky_square,
    &background_camera,
    gpu
  );

  gpu->select_gpu_program(&pyramid_shader);
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
  // gpu->set_fragment_shader_vec3(
  //   &pyramid_shader,
  //   "color",
  //   &COLOR_RED
  // );
  gpu->draw_mesh(&pyramid_mesh);
}
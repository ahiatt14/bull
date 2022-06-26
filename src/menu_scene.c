// TODO: remove
#include <stdio.h>

#include "tail.h"

#include "gpu_helpers.h"
#include "scene.h"
#include "menu_scene.h"
#include "constants.h"

#include "menu_sky.h"

// TODO: temp
#include "square.h"
#include "sky_frag.h"
#include "clod256_texture.h"

#include "burdock_mesh.h"
#include "default_vert.h"
#include "solid_color_frag.h"
#include "normal_debug_frag.h"

#define DELTA_TIME_CAP 1.0f / 30.0f

double delta_time;
double start_time;

struct camera foreground_camera;
struct camera background_camera;

struct transform burdock_transform = {{ 0, 0, 0 }, { 0, 0, 0 }, 1};
struct m4x4 burdock_model;
struct m3x3 burdock_normals_model;

struct square sky_square;

struct gpu_program leaf_shader;

void menu__init(
  struct window_api *window,
  struct viewport *vwprt,
  struct gpu_api *gpu
) {

  gpu->enable_depth_test();
  gpu->cull_no_faces();

  camera__init(&foreground_camera);
  camera__init(&background_camera);

  camera__set_position(0, 0.2f, 0.6f, &foreground_camera);
  camera__set_look_target(&ORIGIN, &foreground_camera);
  camera__set_horizontal_fov_in_deg(80, &foreground_camera);
  camera__set_near_clip_distance(0.2f, &foreground_camera);
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
    // sky_frag_src,
    normal_debug_frag_src,
    &sky_square,
    gpu
  );

  menu_sky__init(gpu);

  leaf_shader.frag_shader_src = normal_debug_frag_src;
  leaf_shader.vert_shader_src = default_vert_src;
  gpu->copy_program_to_gpu(&leaf_shader);

  gpu->copy_static_mesh_to_gpu(&burdock_mesh);
  m4x4__identity(&burdock_model);
}

int vwprt_printed = 0;
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
  burdock_transform.rotation_in_deg.y += 20.0f * delta_time;

  menu_sky__tick(delta_time, seconds_since_creation, gpu);

  // DRAW
  // TODO: update clear to accept vec3*
  gpu->clear(&COLOR_LIGHT_GREY.x);

  menu_sky__draw(
    gpu,
    camera__get_lookat(&background_camera),
    camera__get_perspective(&background_camera)
  );
  gpu->clear_depth_buffer();

  square__draw(
    &sky_square,
    &background_camera,
    gpu
  );

  gpu->select_gpu_program(&leaf_shader);
  space__create_model(&WORLDSPACE, &burdock_transform, &burdock_model);
  space__create_normals_model(&burdock_model, &burdock_normals_model);
  gpu__set_mvp(
    &burdock_model,
    camera__get_lookat(&foreground_camera),
    camera__get_perspective(&foreground_camera),
    &burdock_normals_model,
    &leaf_shader,
    gpu
  );
  gpu->set_fragment_shader_vec3(
    &leaf_shader,
    "color",
    &COLOR_WHITE
  );
  gpu->draw_mesh(&burdock_mesh);
}
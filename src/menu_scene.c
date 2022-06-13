// TODO: remove
#include <stdio.h>

#include "tail.h"
#include "scene.h"
#include "menu_scene.h"
#include "constants.h"

#include "default_vert.h"
#include "solid_color_frag.h"
#include "normal_debug_frag.h"
#include "burdock_mesh.h"
#include "capsule_apartment_mesh.h"

#define DELTA_TIME_CAP 1.0f / 30.0f

double delta_time;
double start_time;

struct camera foreground_camera;
struct camera background_camera;

struct drawable_mesh apartment_mesh;
struct transform apartment_transform = {{ 1, 0, -14 }, { 0, 0, 0 }, 1};
struct m4x4 apartment_model;
struct m3x3 apartment_normals_model;

struct drawable_mesh burdock_mesh;
struct transform burdock_transform = {{ 0, 0, 0 }, { 30, 15, 45 }, 1};
struct m4x4 burdock_model;
struct m3x3 burdock_normals_model;

struct gpu_program leaf_shader;
struct gpu_program apartment_shader;

void menu__init(
  struct window_api *window,
  struct viewport *vwprt,
  struct gpu_api *gpu
) {

  gpu->enable_depth_test();

  camera__init(&foreground_camera);
  camera__init(&background_camera);

  camera__set_position(0, 0, 3, &foreground_camera);
  camera__set_look_target(&ORIGIN, &foreground_camera);
  camera__set_horizontal_fov_in_deg(80, &foreground_camera);
  camera__set_near_clip_distance(0.2f, &foreground_camera);
  camera__set_far_clip_distance(100, &foreground_camera);
  camera__calculate_lookat(&WORLDSPACE.up, &foreground_camera);
  camera__calculate_perspective(vwprt, &foreground_camera);

  leaf_shader.frag_shader_src = solid_color_frag_src;
  leaf_shader.vert_shader_src = default_vert_src;
  gpu->copy_program_to_gpu(&leaf_shader);

  apartment_shader.frag_shader_src = normal_debug_frag_src;
  apartment_shader.vert_shader_src = default_vert_src;
  gpu->copy_program_to_gpu(&apartment_shader);

  apartment_mesh.vertex_buffer = capsule_apartment_vertices;
  apartment_mesh.index_buffer = capsule_apartment_indices;
  apartment_mesh.vertex_buffer_size = sizeof(capsule_apartment_vertices);
  apartment_mesh.index_buffer_size = sizeof(capsule_apartment_indices);
  apartment_mesh.index_buffer_length = capsule_apartment_index_count;
  gpu->copy_mesh_to_gpu(&apartment_mesh);
  m4x4__identity(&apartment_model);

  burdock_mesh.vertex_buffer = burdock_vertices;
  burdock_mesh.index_buffer = burdock_indices;
  burdock_mesh.vertex_buffer_size = sizeof(burdock_vertices);
  burdock_mesh.index_buffer_size = sizeof(burdock_indices);
  burdock_mesh.index_buffer_length = burdock_index_count;
  gpu->copy_mesh_to_gpu(&burdock_mesh);
  m4x4__identity(&burdock_model);
  space__create_model(&WORLDSPACE, &burdock_transform, &burdock_model);
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
  apartment_transform.rotation_in_deg.z += 20.0f * delta_time;

  // DRAW
  gpu->clear(&COLOR_LIGHT_GREY.x);

  gpu->cull_back_faces();
  gpu->select_gpu_program(&apartment_shader);
  space__create_model(&WORLDSPACE, &apartment_transform, &apartment_model);
  space__create_normals_model(&apartment_model, &apartment_normals_model);
  gpu->set_vertex_shader_m4x4(
    &apartment_shader,
    "model",
    &apartment_model
  );
  gpu->set_vertex_shader_m4x4(
    &apartment_shader,
    "lookat",
    camera__get_lookat(&foreground_camera)
  );
  gpu->set_vertex_shader_m3x3(
    &apartment_shader,
    "normals_model",
    &apartment_normals_model
  );
  gpu->set_vertex_shader_m4x4(
    &apartment_shader,
    "perspective",
    camera__get_perspective(&foreground_camera)
  );
  gpu->set_fragment_shader_vec3(
    &apartment_shader,
    "color",
    &COLOR_BLACK
  );
  gpu->draw_mesh(&apartment_mesh);

  gpu->cull_no_faces();
  gpu->select_gpu_program(&leaf_shader);
  gpu->set_vertex_shader_m4x4(
    &leaf_shader,
    "model",
    &burdock_model
  );
  gpu->set_vertex_shader_m4x4(
    &leaf_shader,
    "lookat",
    camera__get_lookat(&foreground_camera)
  );
  gpu->set_vertex_shader_m4x4(
    &leaf_shader,
    "perspective",
    camera__get_perspective(&foreground_camera)
  );
  gpu->set_fragment_shader_vec3(
    &leaf_shader,
    "color",
    &COLOR_WHITE
  );
  gpu->draw_mesh(&burdock_mesh);
}
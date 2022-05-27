#include "tail.h"
#include "scene.h"
#include "burdock.h"
#include "shaders.h"
#include "menu_scene.h"
#include "constants.h"

struct camera foreground_cam;
struct camera background_cam;

struct drawable_mesh burdock_mesh;
struct gpu_program leaf_shader;

void menu__init(struct window_api *window, struct gpu_api *gpu) {

  camera__init(&foreground_cam);
  camera__init(&background_cam);

  gpu->enable_depth_test();
  gpu->cull_no_faces();

  leaf_shader.frag_shader_src = solid_color_frag_shader_src;
  leaf_shader.vert_shader_src = plain_vert_shader_src;
  
  gpu->copy_program_to_gpu(&leaf_shader);

  burdock_mesh.vertex_buffer = burdock_vertices;
  burdock_mesh.index_buffer = burdock_indices;
  burdock_mesh.vertex_buffer_size = sizeof(burdock_vertices);
  burdock_mesh.index_buffer_size = sizeof(burdock_indices);
  burdock_mesh.index_buffer_length = 1140;
  gpu->copy_mesh_to_gpu(&burdock_mesh);
}

void menu__tick(
  double seconds_since_creation,
  const struct viewport *vw,
  struct gpu_api *gpu,
  struct scene **scenes,
  void switch_scene(struct scene* new_scene)
) {
  gpu->clear(COLOR_LIGHT_GREY);

  gpu->select_gpu_program(&leaf_shader);
  gpu->draw_mesh(&burdock_mesh);
}
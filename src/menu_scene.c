#include "tail.h"
#include "scene.h"
#include "burdock.h"
#include "shaders.h"
#include "menu_scene.h"
#include "constants.h"

struct camera foreground_cam;
struct camera background_cam;

struct drawable_mesh burdock_mesh;
void init_burdock_mesh(struct drawable_mesh *dm);


void menu__init(struct window_api *window, struct gpu_api *gpu) {

  camera__init(&foreground_cam);
  camera__init(&background_cam);

  gpu->enable_depth_test();
  gpu->cull_no_faces();

  gpu->copy_mesh_to_gpu(&burdock_mesh);
}

void menu__tick(
  double seconds_since_creation,
  const struct viewport *vw,
  struct gpu_api *gpu,
  struct scene **scenes,
  void switch_scene(struct scene* new_scene)
) {

  gpu->draw_mesh(&burdock_mesh);

  gpu->clear(COLOR_LIGHT_GREY);
}

void init_burdock_mesh(struct drawable_mesh *dm) {
  dm->vertex_buffer = burdock_vertices;
  dm->index_buffer = burdock_indices;
  dm->vertex_buffer_size = sizeof(burdock_vertices);
  dm->index_buffer_size = sizeof(burdock_indices);
  dm->index_buffer_length = 1140;
}
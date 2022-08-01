#include <stdint.h>

#include "tail.h"

#include "quad.h"

#include "flat_texture_frag.h"
#include "default_vert.h"

static uint8_t gpu_has_shared_assets = 0;

static struct drawable_mesh shared_mesh = {
  .vertices = (struct vertex [4]){
    {{ -0.5f, 0, 0.5f },{0, 1, 0},{0, 0}},
    {{ 0.5f, 0, 0.5f },{0, 1, 0},{1, 0}},
    {{ 0.5f, 0, -0.5f },{0, 1, 0},{1, 1}},
    {{ -0.5f, 0, -0.5f },{0, 1, 0},{0, 1}}
  },
  .indices = (unsigned int[6]){
    0, 1, 2,
    0, 2, 3
  },
  .vertices_size = sizeof(struct vertex) * 4,
  .indices_size = sizeof(unsigned int) * 6,
  .indices_length = 6
};

static struct shader shared_shader;

static struct m4x4 shared_local_to_world;

void quad__copy_assets_to_gpu(
  struct gpu_api const *const gpu,
  struct quad *const qua
) {
  gpu->copy_rgb_texture_to_gpu(qua->texture);
  if (gpu_has_shared_assets) return;
  shared_shader.frag_shader_src = flat_texture_frag_src;
  shared_shader.vert_shader_src = default_vert_src;
  gpu->copy_shader_to_gpu(&shared_shader);
  gpu->copy_dynamic_mesh_to_gpu(&shared_mesh);
}

void quad__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct quad const *const qua
) {
  

  gpu->select_shader(&shared_shader);
  gpu->select_texture(qua->texture);
  gpu->set_vertex_shader_m4x4(
    &shared_shader,
    "model",
    &shared_local_to_world
  );
  gpu->set_vertex_shader_m4x4(
    &shared_shader,
    "view",
    camera__get_lookat(cam)
  );
  gpu->set_vertex_shader_m4x4(
    &shared_shader,
    "projection",
    camera__get_perspective(cam)
  );
  gpu->draw_mesh(&shared_mesh);
}
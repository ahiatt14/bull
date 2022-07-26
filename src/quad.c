#include "tail.h"

#include "quad.h"

#include "flat_texture_frag.h"
#include "default_vert.h"

static struct shader shared_shader;

static struct m4x4 shared_local_to_world;

void quad__create(
  struct vec2 size,
  struct quad *const qua
) {
  
}

void quad__copy_assets_to_gpu(
  struct gpu_api const *const gpu,
  struct quad *const qua
) {
  shared_shader.frag_shader_src = flat_texture_frag_src;
  shared_shader.vert_shader_src = default_vert_src;
  gpu->copy_shader_to_gpu(&shared_shader);
  gpu->copy_static_mesh_to_gpu(&)
}

void quad__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct quad const *const qua
) {
  gpu->select_shader(&shared_shader);
}
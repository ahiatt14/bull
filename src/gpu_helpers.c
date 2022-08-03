#include "tail.h"

#include "gpu_helpers.h"
#include "constants.h"

// TODO: even worth it?
void gpu__set_mvp(
  struct m4x4 const *const model,
  struct m3x3 const *const normals_model,
  struct camera const *const cam,
  struct shader const *const shad,
  struct gpu_api const *const gpu
) {
  gpu->set_vertex_shader_m3x3(shad, "normals_model", normals_model);
  gpu->set_vertex_shader_m4x4(shad, "model", model);
  gpu->set_vertex_shader_m4x4(shad, "view", &cam->lookat);
  gpu->set_vertex_shader_m4x4(shad, "perspective", &cam->perspective);
}
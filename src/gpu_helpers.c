#include "tail.h"

#include "gpu_helpers.h"

void gpu__set_mvp(
  const struct m4x4 *model,
  const struct m4x4 *view,
  const struct m4x4 *perspective,
  const struct m3x3 *normals_model,
  const struct gpu_program *prog,
  const struct gpu_api *gpu
) {
  gpu->set_vertex_shader_m3x3(prog, "normals_model", normals_model);
  gpu->set_vertex_shader_m4x4(prog, "model", model);
  gpu->set_vertex_shader_m4x4(prog, "view", view);
  gpu->set_vertex_shader_m4x4(prog, "perspective", perspective);
}
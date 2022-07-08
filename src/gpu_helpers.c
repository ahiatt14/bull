#include "tail.h"

#include "gpu_helpers.h"
#include "constants.h"

void gpu__set_mvp(
  struct m4x4 const *const model,
  struct m3x3 const *const normals_model,
  struct camera const *const cam,
  struct shader const *const shad,
  struct gpu_api const *const gpu
) {
  gpu->set_vertex_shader_m3x3(shad, "normals_model", normals_model);
  gpu->set_vertex_shader_m4x4(shad, "model", model);
  gpu->set_vertex_shader_m4x4(shad, "view", camera__get_lookat(cam));
  gpu->set_vertex_shader_m4x4(
    shad,
    "perspective",
    camera__get_perspective(cam)
  );
}
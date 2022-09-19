#include <stdint.h>

#include "tail.h"

#include "tail_helpers.h"
#include "constants.h"

// TODO: even worth it?
void gpu__set_mvp(
  struct m4x4 const *const model,
  struct m3x3 const *const normals_model,
  struct camera const *const cam,
  struct shader const *const shad,
  struct gpu_api const *const gpu
) {
  gpu->set_shader_m3x3(shad, "normals_model", normals_model);
  gpu->set_shader_m4x4(shad, "model", model);
  gpu->set_shader_m4x4(shad, "view", &cam->lookat);
  gpu->set_shader_m4x4(shad, "projection", &cam->projection);
}

void mesh__tile_uvs(
  float x_multiplier,
  float y_multiplier,
  struct drawable_mesh *const mesh
) {
  for (uint16_t i = 0; i < mesh->vertices_length; i++) {
    mesh->vertices[i].uv.x *= x_multiplier;
    mesh->vertices[i].uv.y *= y_multiplier;
  }
}

// TODO: need a deep dive on this, not sure we're ending up where we want
struct vec3 space__world_to_ndc(
  struct camera const *const cam,
  struct vec3 pos
) {
  static struct m4x4 view_x_projection;
  m4x4_x_m4x4(
    &cam->lookat,
    &cam->projection,
    &view_x_projection
  );
  return m4x4_x_point(
    &view_x_projection,
    pos
  );
}
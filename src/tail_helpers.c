#include <stdint.h>

#include "tail.h"

#include "tail_helpers.h"
#include "constants.h"

// TODO: even worth it?
void gpu__set_mvp(
  M4x4 const *const model,
  M3x3 const *const normals_model,
  Camera const *const cam,
  Shader const *const shad,
  GPU const *const gpu
) {
  gpu->set_shader_m3x3(shad, "normals_model", normals_model);
  gpu->set_shader_m4x4(shad, "model", model);
  gpu->set_shader_m4x4(shad, "view", &cam->lookat);
  gpu->set_shader_m4x4(shad, "projection", &cam->projection);
}

void mesh__tile_uvs(
  float x_multiplier,
  float y_multiplier,
  DrawableMesh *const mesh
) {
  for (uint16_t i = 0; i < mesh->vertices_length; i++) {
    mesh->vertices[i].uv.x *= x_multiplier;
    mesh->vertices[i].uv.y *= y_multiplier;
  }
}
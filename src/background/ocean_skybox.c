#include "tail.h"

#include "ocean_skybox.h"

#include "assets.h"
#include "constants.h"
#include "tail_helpers.h"

#include "cubemap_mesh.h"

void ocean_skybox__copy_assets_to_gpu(
  GPU const *const gpu
) {
  gpu->copy_static_mesh_to_gpu(&CUBEMAP_MESH);
}

void draw_ocean_skybox(
  Camera const *const camera,
  GPU const *const gpu
) {

  gpu->select_shader(&SKYBOX_SHADER);
  gpu->select_cubemap(&OCEAN_SKYBOX);

  gpu->set_shader_m4x4(&SKYBOX_SHADER, "view", &camera->lookat);
  gpu->set_shader_m4x4(&SKYBOX_SHADER, "projection", &camera->projection);

  gpu->draw_mesh(&CUBEMAP_MESH);

  gpu->clear_depth_buffer();
}
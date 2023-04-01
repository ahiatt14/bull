#include <math.h>

#include "tail.h"

#include "ocean_skybox.h"

#include "assets.h"
#include "constants.h"
#include "tail_helpers.h"

#include "cubemap_mesh.h"

#define ROTATE_SPEED -0.01f

void ocean_skybox__copy_assets_to_gpu(
  GPU const *const gpu
) {
  gpu->copy_static_mesh_to_gpu(&CUBEMAP_MESH);
}

void draw_ocean_skybox(
  GameTime time,
  Camera const *const camera,
  GPU const *const gpu
) {

  gpu->select_shader(&SKYBOX_SHADER);
  gpu->select_cubemap(&OCEAN_SKYBOX);

  static M4x4 rotation;
  static M3x3 rotation_uniform;
  m4x4__rotation(
    ROTATE_SPEED * time.sec_since_game_launch,
    WORLDSPACE.up,
    &rotation
  );
  m4x4__sub3x3_from00(&rotation, &rotation_uniform);

  gpu->set_shader_m3x3(&SKYBOX_SHADER, "rotation", &rotation_uniform);
  gpu->set_shader_m4x4(&SKYBOX_SHADER, "view", &camera->lookat);
  gpu->set_shader_m4x4(&SKYBOX_SHADER, "projection", &camera->projection);

  gpu->draw_mesh(&CUBEMAP_MESH);

  gpu->clear_depth_buffer();
}
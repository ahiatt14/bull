#include <math.h>

#include "tail.h"

#include "ocean_skybox.h"

#include "assets.h"
#include "constants.h"
#include "tail_helpers.h"

static M4x4 rotation;
static M3x3 rotation_uniform;

void ocean_skybox__copy_assets_to_gpu(
  GPU const *const gpu
) {
  m4x4__rotation(
    M_PI,
    WORLDSPACE.up,
    &rotation
  );
  m4x4__sub3x3_from00(&rotation, &rotation_uniform);
}

void draw_ocean_skybox(
  GameTime time,
  Camera const *const camera,
  GPU const *const gpu
) {

  gpu->select_shader(&SKYBOX_SHADER);
  gpu->select_cubemap(&OCEAN_SKYBOX);

  gpu->set_shader_m3x3(&SKYBOX_SHADER, "rotation", &rotation_uniform);
  gpu->set_shader_m4x4(&SKYBOX_SHADER, "view", &camera->lookat);
  gpu->set_shader_m4x4(&SKYBOX_SHADER, "projection", &camera->projection);

  gpu->draw_mesh(&CUBEMAP_MESH);

  gpu->clear_depth_buffer();
}
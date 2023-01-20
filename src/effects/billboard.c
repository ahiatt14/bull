#include <math.h>

#include "tail.h"

#include "billboard.h"

#include "constants.h"
#include "bull_math.h"

#include "billboard_vert.h"
#include "flat_texture_frag.h"

static struct shader shad;

void billboards__copy_assets_to_gpu(
  struct gpu_api const *const gpu
) {
  shad.frag_src = flat_texture_frag_src;
  shad.vert_src = billboard_vert_src;
  gpu->copy_shader_to_gpu(&shad);
}

void billboard__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct billboard *const bb
) {

  gpu->select_shader(&shad);
  gpu->select_texture(bb->texture);

  gpu->set_shader_vec3(&shad, "center_pos_world", bb->transform.position);
  gpu->set_shader_float(&shad, "scale", bb->transform.scale);

  gpu->set_shader_m4x4(&shad, "view", &cam->lookat);
  gpu->set_shader_m4x4(&shad, "projection", &cam->projection);
  gpu->draw_mesh(&QUAD);
}
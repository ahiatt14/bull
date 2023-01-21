#include <math.h>

#include "tail.h"

#include "billboard.h"

#include "constants.h"
#include "bull_math.h"

#include "billboard_vert.h"
#include "billboard_geo.h"
#include "flat_texture_frag.h"

static struct shader shad;

static struct point_buffer billboard_center = {
  .points = (struct vec3[1]){{ 0, 0, 0 }},
  .points_length = 1
};

void billboard__copy_shared_assets_to_gpu(
  struct gpu_api const *const gpu
) {
  gpu->copy_points_to_gpu(&billboard_center);
  shad.frag_src = flat_texture_frag_src;
  shad.vert_src = billboard_vert_src;
  shad.geo_src = billboard_geo_src;
  gpu->copy_shader_to_gpu(&shad);
}

void billboard__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct billboard *const bb
) {

  gpu->select_shader(&shad);
  gpu->select_texture(bb->texture);

  gpu->set_shader_float(&shad, "scale", bb->transform.scale);

  static struct m4x4 translation;
  m4x4__translation(bb->transform.position, &translation);

  gpu->set_shader_m4x4(&shad, "model", &translation);
  gpu->set_shader_m4x4(&shad, "view", &cam->lookat);
  gpu->set_shader_m4x4(&shad, "projection", &cam->projection);

  gpu->draw_points(&billboard_center);
}
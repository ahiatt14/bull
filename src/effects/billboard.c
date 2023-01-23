#include "tail.h"

#include "billboard.h"

#include "constants.h"

void billboard__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct shader const *const shad,
  struct vec3 position,
  float scale
) {

  static struct m4x4 translation;
  m4x4__translation(position, &translation);

  gpu->set_shader_float(shad, "scale", scale);

  gpu->set_shader_m4x4(shad, "model", &translation);
  gpu->set_shader_m4x4(shad, "view", &cam->lookat);
  gpu->set_shader_m4x4(shad, "projection", &cam->projection);

  gpu->draw_points(&POINT);
}
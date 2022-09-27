#include "tail.h"

#include "fireballs.h"

#include "tail_helpers.h"
#include "constants.h"
#include "bull_math.h"

#include "lowpoly_sphere_mesh.h"

#define MAX_FIREBALLS_COUNT 50

static struct fireball fireballs[MAX_FIREBALLS_COUNT];

static uint_fast16_t highest_active_index;

void fireballs__copy_assets_to_gpu(
  struct gpu_api const *const gpu
) {
  gpu->copy_static_mesh_to_gpu(&lowpoly_sphere_mesh);
}

void fireballs__reset_state() {
  highest_active_index = 0;
}

void fireballs__revolve(
  struct gametime time,
  float deg_per_sec
) {

}

void fireballs__activate_fireball(
  struct vec3 position,
  int_fast8_t ccw_coefficient
) {

}

void fireballs__deactivate_fireball(
  int_fast16_t index_to_deactivate
) {
  struct fireball temp = fireballs[highest_active_index];
  highest_active_index--;
  fireballs[index_to_deactivate] = temp;
}

void fireballs__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu
) {
  static struct m4x4 translation;

  gpu->select_shader(&SOLID_COLOR_SHADER);
  gpu->set_shader_vec3(
    &SOLID_COLOR_SHADER,
    "color",
    COLOR_GOLDEN_YELLOW
  );
  for (int i = 0; i < highest_active_index; i++) {
    m4x4__translation(
      battlefield_to_world_pos(fireballs[i].position),
      &translation
    );
    gpu__set_mvp(
      &translation,
      &M3X3_IDENTITY,
      cam,
      &SOLID_COLOR_SHADER,
      gpu
    );
    gpu->draw_mesh(&lowpoly_sphere_mesh);
  }
}
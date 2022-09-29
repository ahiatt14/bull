#include "tail.h"

#include "fireballs.h"

#include "tail_helpers.h"
#include "constants.h"
#include "bull_math.h"

#include "lowpoly_sphere_mesh.h"

#define MAX_FIREBALL_COUNT 100

struct fireball {
  struct battlefield_pos position;
  float starting_battlefield_deg;
  double sec_since_activation;
  int8_t ccw_coefficient; // 1 for ccw, -1 for cw (?)
};

static struct fireball fireballs[MAX_FIREBALL_COUNT];
static uint_fast16_t highest_active_index;

void fireballs__copy_assets_to_gpu(
  struct gpu_api const *const gpu
) {
  gpu->copy_static_mesh_to_gpu(&lowpoly_sphere_mesh);
}

void fireballs__deactivate_all() {
  highest_active_index = 0;
}

void fireballs__revolve(
  struct gametime time,
  double sec_per_revolution
) {
  for (int i = 0; i < highest_active_index; i++) {
    fireballs[i].sec_since_activation += time.delta;
    fireballs[i].position.degrees +=
    360 *
    (fireballs[i].sec_since_activation / sec_per_revolution) *
    fireballs[i].ccw_coefficient;
  }
}

void fireballs__activate_fireball(
  struct vec3 position,
  int_fast8_t ccw_coefficient
) {
  // NOTE: should never happen
  if (highest_active_index == MAX_FIREBALL_COUNT - 1) return;
  highest_active_index++;
  struct battlefield_pos bfpos = world_to_battlefield_pos(position);
  fireballs[highest_active_index] = (struct fireball){
    .position = bfpos,
    .starting_battlefield_deg = bfpos.degrees,
    .ccw_coefficient = ccw_coefficient
  };
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
  gpu->set_shader_vec3(&SOLID_COLOR_SHADER, "color", COLOR_BLOOD_RED);
  for (int i = 0; i < highest_active_index; i++) {
    // TODO: implement batch meshes to reduce draw calls!
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
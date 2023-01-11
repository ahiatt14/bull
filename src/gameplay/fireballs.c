#include <stdio.h>
#include <math.h>

#include "tail.h"

#include "fireballs.h"

#include "tail_helpers.h"
#include "constants.h"
#include "bull_math.h"

#include "lowpoly_sphere_smooth_mesh.h"

#define MAX_FIREBALL_COUNT 20

struct fireball {
  struct battlefield_pos position;
  double sec_since_activation;
};

static struct fireball fireballs[MAX_FIREBALL_COUNT];
static uint_fast16_t highest_active_index;

void fireballs__copy_assets_to_gpu(
  struct gpu_api const *const gpu
) {
  gpu->copy_static_mesh_to_gpu(&lowpoly_sphere_smooth_mesh);
}

void fireballs__deactivate_all() {
  highest_active_index = 0;
}

void fireballs__move(
  struct gametime time,
  float world_unit_per_second,
  float max_radius
) {
  for (int i = 0; i < highest_active_index; i++) {
    fireballs[i].sec_since_activation += time.delta;
    fireballs[i].position.radius -= world_unit_per_second * time.delta;
  }
}

// void fireballs__revolve(
//   struct gametime time,
//   double revolutions_per_sec
// ) {
//   for (int i = 0; i < highest_active_index; i++) {
//     fireballs[i].sec_since_activation += time.delta;
//     fireballs[i].position.degrees +=
//       revolutions_per_sec * 360.0f *
//       time.delta *
//       fireballs[i].ccw_coefficient;
//   }
// }

void fireballs__activate_fireball(
  struct battlefield_pos bfpos
) {
  uint_fast16_t resolved_index =
    highest_active_index + 1 >= MAX_FIREBALL_COUNT ?
    highest_active_index :
    highest_active_index++;

  fireballs[resolved_index] = (struct fireball){
    .position = bfpos,
    .sec_since_activation = 0
  };
  
  printf("highest index: %u\n", resolved_index);
  printf(
    "latest fireball: radius: %f ; deg: %f\n",
    fireballs[resolved_index].position.radius,
    fireballs[resolved_index].position.degrees
  );
}

void fireballs__deactivate_fireball(
  int_fast16_t index_to_deactivate
) {

  fireballs[index_to_deactivate] = fireballs[highest_active_index--];
}

void fireballs__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu
) {
  static struct m4x4 translation;

  gpu->select_shader(&SOLID_COLOR_SHADER);
  gpu->set_shader_vec3(&SOLID_COLOR_SHADER, "color", COLOR_GOLDEN_YELLOW);
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
    gpu->draw_mesh(&lowpoly_sphere_smooth_mesh);
  }
}
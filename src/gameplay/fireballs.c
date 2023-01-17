#include <stdio.h>
#include <math.h>

#include "tail.h"

#include "fireballs.h"

#include "tail_helpers.h"
#include "constants.h"
#include "bull_math.h"

#include "lowpoly_sphere_smooth_mesh.h"

static struct fireball* get_fireball(
  uint_fast16_t from_start,
  struct fireballs *const fbs
) {
  return &(fbs->_ring_buffer[(fbs->_tail + from_start) % MAX_FIREBALLS]);
}

void fireballs__copy_assets_to_gpu(
  struct gpu_api const *const gpu
) {
  gpu->copy_static_mesh_to_gpu(&lowpoly_sphere_smooth_mesh);
}

void fireballs__deactivate_all(
  struct fireballs *const fbs
) {
  fbs->_tail = 0;
  fbs->_active_count = 0;
}

// TODO: much optimization possible
void fireballs__move(
  struct gametime time,
  float world_unit_per_second,
  float max_radius,
  struct fireballs *const fbs
) {

  for (int i = 0; i < fbs->_active_count; i++) {

    static struct fireball *fb;
    fb = get_fireball(i, fbs);

    fb->sec_since_activation += time.delta;
    fb->position.radius -= world_unit_per_second * time.delta;
  }
}

void fireballs__deactivate(
  uint_fast16_t from_start,
  struct fireballs *const fbs
) {
  uint_fast16_t index_to_replace = (fbs->_tail + from_start) % MAX_FIREBALLS;
  fbs->_ring_buffer[index_to_replace] = fbs->_ring_buffer[fbs->_tail];
  fbs->_tail = (fbs->_tail + 1) % MAX_FIREBALLS;
  fbs->_active_count--;
}

void fireballs__activate (
  struct battlefield_pos bfpos,
  struct fireballs *const fbs
) {
  if (fbs->_active_count == MAX_FIREBALLS) {
    fbs->_ring_buffer[fbs->_tail] = (struct fireball){
      .position = bfpos,
      .sec_since_activation = 0
    };
    fbs->_tail = (fbs->_tail + 1) % MAX_FIREBALLS;
  } else {
    fbs->_ring_buffer[
      (fbs->_tail + fbs->_active_count) % MAX_FIREBALLS
    ] = (struct fireball){
      .position = bfpos,
      .sec_since_activation = 0
    };
    fbs->_active_count++;
  }
}

void fireballs__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct fireballs *const fbs
) {

  gpu->select_shader(&SOLID_COLOR_SHADER);
  gpu->set_shader_vec3(&SOLID_COLOR_SHADER, "color", COLOR_GOLDEN_YELLOW);

  for (int i = 0; i < fbs->_active_count; i++) {

    static struct fireball *fb;
    fb = get_fireball(i, fbs);

    static struct m4x4 translation;
    m4x4__translation(
      battlefield_to_world_pos(fb->position),
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
#include <stdio.h>
#include <math.h>

#include "tail.h"

#include "fireballs.h"

#include "tail_helpers.h"
#include "constants.h"
#include "bull_math.h"

#include "lowpoly_sphere_smooth_mesh.h"

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

void fireballs__move(
  struct gametime time,
  float world_unit_per_second,
  float max_radius,
  struct fireballs *const fbs
) {
  static unsigned int index, tail, active_count;
  tail = fbs->_tail;
  active_count = fbs->_active_count;

  for (int i = 0; i < active_count; i++) {
    index = (tail + i) % MAX_FIREBALLS;
    fbs->_ring_buffer[index].sec_since_activation +=
      time.delta;
    fbs->_ring_buffer[index].position.radius -=
      world_unit_per_second * time.delta;
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
  struct fireballs const *const fbs
) {
  static struct fireball fb;
  static struct m4x4 translation;

  gpu->select_shader(&SOLID_COLOR_SHADER);
  gpu->set_shader_vec3(&SOLID_COLOR_SHADER, "color", COLOR_GOLDEN_YELLOW);

  for (int i = 0; i < fbs->_active_count; i++) {

    fb = fbs->_ring_buffer[(fbs->_tail + fbs->_active_count) % MAX_FIREBALLS];

    m4x4__translation(
      battlefield_to_world_pos(fb.position),
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

#include <stdio.h>
#include <math.h>

#include "tail.h"

#include "bouncers.h"

#include "constants.h"
#include "bull_math.h"
#include "gpu_helpers.h"

#include "sphere_mesh.h"

#include "bouncer_frag.h"
#include "solid_color_frag.h"
#include "default_vert.h"

static struct shader shared_bouncer_shader;

static uint8_t off_at_place(uint8_t place, uint32_t state) {
  return (state >> place) & 1 ? 0 : 1;
}

static uint32_t switch_bit_on(uint8_t place, uint32_t state) {
  return state | 1 << place;
}

static uint32_t switch_bit_off(uint8_t place, uint32_t state) {
  return state & ~(1 << place);
}

void bouncers__copy_assets_to_gpu(
  struct gpu_api const *const gpu
) {
  shared_bouncer_shader.frag_shader_src = bouncer_frag_src;
  shared_bouncer_shader.vert_shader_src = default_vert_src;
  gpu->copy_shader_to_gpu(&shared_bouncer_shader);
  gpu->copy_static_mesh_to_gpu(&sphere_mesh);
}

struct vec3 bouncers__get_pos_of_grid_bouncer(
  uint8_t row,
  uint8_t column,
  struct bouncer_grid const *const grid
) {
  static struct vec3 position;
  static struct m4x4 rotation;

  position.z = -(
    grid->row_0_radius_offset +
    row * BOUNCER_GRID_ROW_RADIUS_OFFSET
  );

  m4x4__rotation(
    deg_to_rad(
      grid->row_deg_offsets[row] +
      column * BOUNCER_GRID_COLUMN_DEG_OFFSET
    ),
    WORLDSPACE.up,
    &rotation
  );
  return m4x4_x_point(&rotation, position);
}

void bouncers__reset_grid_row(
  uint8_t row,
  struct bouncer_grid *const grid
) {
  grid->active_bouncers[row] = 0;
  grid->row_deg_offsets[row] = 0;
}

void bouncers__set_grid_to_defaults(
  struct bouncer_grid *const grid
) {
  grid->row_0_radius_offset = 0;
  for (int i = 0; i < BOUNCERS_GRID_ROW_COUNT; i++)
    bouncers__reset_grid_row(i, grid);
}

void bouncers__add_to_grid(
  uint8_t row,
  uint8_t column,
  struct bouncer_grid *const grid
) {
  grid->active_bouncers[row] =
    switch_bit_on(column, grid->active_bouncers[row]);
}

void bouncers__delete_from_grid(
  uint8_t row,
  uint8_t column,
  struct bouncer_grid *const grid
) {
  grid->active_bouncers[row] =
    switch_bit_off(column, grid->active_bouncers[row]);
}

void bouncers__check_collision_with_grid(
  void (*on_collide)(
    uint8_t row,
    uint8_t column,
    struct vec3 bouncer_to_target,
    struct bouncer_grid *const grid
  ),
  struct vec3 target,
  struct bouncer_grid *const grid
) {
  static struct vec3 bouncer_position;
  static float target_distance_from_center;

  target_distance_from_center = vec3__distance(target, ORIGIN);
  if (
    target_distance_from_center <
    grid->row_0_radius_offset - BOUNCER_RADIUS
  ) return;
  
  for (int_fast8_t r = 0; r < BOUNCERS_GRID_ROW_COUNT; r++) {
    if (
      grid->active_bouncers[r] == 0 ||
      fabs(
        target_distance_from_center -
        (grid->row_0_radius_offset + r * BOUNCER_GRID_ROW_RADIUS_OFFSET)
      ) > BOUNCER_RADIUS
    ) continue;
    for (int_fast8_t c = 0; c < BOUNCER_GRID_MAX_PER_ROW; c++) {
      bouncer_position = bouncers__get_pos_of_grid_bouncer(r, c, grid);
      if (
        vec3__distance(target, bouncer_position) > BOUNCER_RADIUS ||
        off_at_place(c, grid->active_bouncers[r])
      ) continue;
      on_collide(r, c, vec3_minus_vec3(target, bouncer_position), grid);
      return;
    }
  }
}

void bouncers__radiate_grid(
  float meters_per_second,
  double delta_time,
  struct bouncer_grid *const grid
) {
  grid->row_0_radius_offset += meters_per_second * delta_time;
}

void bouncers__rotate_grid_row(
  uint8_t row,
  float deg_per_sec,
  double delta_time,
  struct bouncer_grid *const grid
) {
  grid->row_deg_offsets[row] = loop_float(
    grid->row_deg_offsets[row] += deg_per_sec * delta_time,
    0,
    360
  );
}

void bouncers__draw_grid(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct bouncer_grid *const grid
) {
  static struct vec3 revolved_position;
  static struct m4x4 local_to_world;
  static struct m3x3 normals_local_to_world;

  gpu->select_shader(&shared_bouncer_shader);
  gpu->set_fragment_shader_vec3(
    &shared_bouncer_shader,
    "core_color",
    COLOR_NEON_PURPLE
  );
  gpu->set_fragment_shader_vec3(
    &shared_bouncer_shader,
    "edge_color",
    COLOR_WHITE
  );

  for (int_fast8_t r = 0; r < BOUNCERS_GRID_ROW_COUNT; r++)
  for (int_fast8_t c = 0; c < BOUNCER_GRID_MAX_PER_ROW; c++) {

    if (off_at_place(c, grid->active_bouncers[r])) continue;

    revolved_position = bouncers__get_pos_of_grid_bouncer(r, c, grid);

    m4x4__translation(
      &revolved_position,
      &local_to_world
    );

    space__create_normals_model(
      &local_to_world,
      &normals_local_to_world
    );
    gpu__set_mvp(
      &local_to_world,
      &normals_local_to_world,
      cam,
      &shared_bouncer_shader,
      gpu
    );
    gpu->draw_mesh(&sphere_mesh);
  }
}
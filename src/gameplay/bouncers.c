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
  return (struct vec3){0};
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
  grid->active_bouncers[row] |= 1 << column;
}

void bouncers__check_collision_with_grid(
  void (*on_collide)(
    uint8_t bouncer_row,
    uint8_t bouncer_column,
    struct vec3 bouncer_to_target,
    struct vec3 bouncer_velocity
  ),
  struct vec3 target,
  struct bouncer_grid const *const grid
) {
  static float target_distance_from_center;

  target_distance_from_center = vec3__distance(target, ORIGIN);
  if (
    target_distance_from_center <
    grid->row_0_radius_offset - BOUNCER_RADIUS
    // TODO: will need to update since we'll make bouncer radius dynamic
    // based on distance from center
  ) return;
  
  for (int_fast8_t r = 0; r < BOUNCERS_GRID_ROW_COUNT; r++) {
    if (
      abs(
        target_distance_from_center -
        grid->row_0_radius_offset + r * BOUNCER_GRID_ROW_RADIUS_OFFSET
      ) >
      BOUNCER_RADIUS
    ) continue;
    for (int_fast8_t c = 0; c < 360 / BOUNCER_GRID_COLUMN_DEG_OFFSET; c++) {

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

static uint8_t bouncer_is_inactive(
  uint64_t bouncer_row,
  int_fast8_t index
) {
  return (bouncer_row >> index) & 1 ? 0 : 1;
}

void bouncers__draw_grid(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct bouncer_grid *const grid
) {
  static struct vec3 position;
  static struct m4x4 rotation;
  static struct vec3 rotated_position; 
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
    COLOR_GOLDEN_YELLOW
  );

  for (int_fast8_t r = 0; r < BOUNCERS_GRID_ROW_COUNT; r++)
  for (int_fast8_t c = 0; c < 360 / BOUNCER_GRID_COLUMN_DEG_OFFSET; c++) {

    if (bouncer_is_inactive(grid->active_bouncers[r], c)) continue;

    position.z = -(
      grid->row_0_radius_offset +
      r * BOUNCER_GRID_ROW_RADIUS_OFFSET
    );

    m4x4__rotation(
      deg_to_rad(
        grid->row_deg_offsets[r] +
        c * BOUNCER_GRID_COLUMN_DEG_OFFSET
      ),
      WORLDSPACE.up,
      &rotation
    );
    rotated_position = m4x4_x_point(&rotation, position);
    m4x4__translation(
      &rotated_position,
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
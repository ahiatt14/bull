#ifndef __BULL_BOUNCERS__
#define __BULL_BOUNCERS__

#define BOUNCER_RADIUS 0.4f

#define BOUNCER_GRID_COLUMN_DEG_OFFSET 10
#define BOUNCER_GRID_ROW_RADIUS_OFFSET 1
#define BOUNCERS_GRID_ROW_COUNT 20
#define BOUNCERS_GRID_MAX_COUNT (360 / BOUNCER_GRID_COLUMN_DEG_OFFSET) * BOUNCERS_GRID_ROW_COUNT

struct bouncer_grid {
  uint64_t active_bouncers[BOUNCERS_GRID_ROW_COUNT];
  float row_deg_offsets[BOUNCERS_GRID_ROW_COUNT];
  float row_0_radius_offset;
};

void bouncers__copy_assets_to_gpu(
  struct gpu_api const *const gpu
);

struct vec3 bouncers__get_pos_of_grid_bouncer(
  uint8_t row,
  uint8_t column,
  struct bouncer_grid const *const grid
);

void bouncers__set_grid_to_defaults(
  struct bouncer_grid *const grid
);

void bouncers__add_to_grid(
  uint8_t row,
  uint8_t column,
  struct bouncer_grid *const grid
);

void bouncers__delete_from_grid(
  uint8_t row,
  uint8_t column,
  struct bouncer_grid *const grid
);

void bouncers__reset_grid_row(
  uint8_t row_index,
  struct bouncer_grid *const grid
);

void bouncers__check_collision_with_grid(
  void (*on_collide)(
    uint8_t row,
    uint8_t column,
    struct vec3 bouncer_to_target,
    struct bouncer_grid *const grid
  ),
  struct vec3 target,
  struct bouncer_grid *const grid
);

void bouncers__rotate_grid_row(
  uint8_t row,
  float deg_per_sec,
  double delta_time,
  struct bouncer_grid *const grid
);

void bouncers__radiate_grid(
  float meters_per_sec,
  double delta_time,
  struct bouncer_grid *const grid
);

void bouncers__draw_grid(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct bouncer_grid *const grid
);

#endif
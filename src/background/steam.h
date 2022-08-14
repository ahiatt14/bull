#ifndef __STEAM_COLUMNS__
#define __STEAM_COLUMNS__

#define STEAM__COLUMN_LVL_COUNT 3

struct steam_column {
  struct vec3 position;
  struct vec3 ring_offsets[STEAM__COLUMN_LVL_COUNT];
  float ring_radii[STEAM__COLUMN_LVL_COUNT];
};

void steam__init_mesh_data();

void steam__copy_assets_to_gpu(
  struct gpu_api const *const gpu
);

void steam__column_default(
  struct steam_column *const column
);

void steam__rise(
  double delta_time,
  double seconds_since_creation,
  struct steam_column *const column
);

void steam__draw_column(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct steam_column *const column
);

#endif
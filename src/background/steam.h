#ifndef __STEAM_COLUMNS__
#define __STEAM_COLUMNS__

#define STEAM__LVL_COUNT 15

struct steam_column {
  struct vec3 position;
  struct vec3 ring_offsets[STEAM__LVL_COUNT];
  float ring_radii[STEAM__LVL_COUNT];
  uint8_t shape_index_offset;
};

void steam__shared_init_mesh_data();

void steam__copy_assets_to_gpu(
  struct gpu_api const *const gpu
);

void steam__column_default(
  struct vec2 wind_km_per_sec,
  struct steam_column *const column
);

void steam__rise(
  double delta_time,
  struct vec2 wind_km_per_sec,
  struct steam_column *const column
);

void steam__draw_column(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct vec3 light_direction,
  struct steam_column *const column
);

#endif
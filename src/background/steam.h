#ifndef __STEAM_COLUMNS__
#define __STEAM_COLUMNS__

#define STEAM__VERTS_PER_LEVEL 6
#define STEAM__LEVEL_VERT_DEG_OFFSET 360.0f / STEAM__VERTS_PER_LEVEL
#define STEAM__LEVEL_COUNT 10
#define STEAM__VERT_COUNT STEAM__VERTS_PER_LEVEL * STEAM__LEVEL_COUNT 
#define STEAM__INDEX_COUNT STEAM__VERTS_PER_LEVEL * 6 * (STEAM__LEVEL_COUNT - 1)

struct steam_column {
  struct vec3 position;
  struct vec3 ring_offsets[STEAM__LEVEL_COUNT];
  float ring_radii[STEAM__LEVEL_COUNT];
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
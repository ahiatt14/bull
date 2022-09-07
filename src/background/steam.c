#include <stdio.h>

#include <stdint.h>
#include <math.h>

#include "tail.h"
#include "steam.h"

#include "gpu_helpers.h"
#include "constants.h"
#include "bull_math.h"

#include "default_vert.h"
#include "steam_frag.h"

#include "core_frag.h"
#include "turbine_frag.h"

#define BOUYANCY 2
#define VERTS_PER_LVL 12
#define LVL_HEIGHT 0.7f
#define MIN_RING_RADII 0.01f
#define RING_VERT_DEG_OFFSET 360.0f / VERTS_PER_LVL
#define VERT_COUNT VERTS_PER_LVL * STEAM__LVL_COUNT
#define INDEX_COUNT VERTS_PER_LVL * 6 * (STEAM__LVL_COUNT - 1)
#define MAX_COLUMN_HEIGHT STEAM__LVL_COUNT * LVL_HEIGHT

static struct shader shared_steam_shader;

static struct drawable_mesh shared_column_mesh = (struct drawable_mesh){
  .vertices = (struct vertex[VERT_COUNT]){0},
  .indices = (unsigned int[INDEX_COUNT]){0},
  .vertices_size = sizeof(struct vertex) * VERT_COUNT,
  .indices_size = sizeof(unsigned int) * INDEX_COUNT,
  .indices_length = INDEX_COUNT
};

void steam__copy_assets_to_gpu(
  struct gpu_api const *const gpu
) {

  shared_steam_shader.frag_shader_src = steam_frag_src;
  shared_steam_shader.vert_shader_src = default_vert_src;
  gpu->copy_shader_to_gpu(&shared_steam_shader);

  gpu->copy_dynamic_mesh_to_gpu(&shared_column_mesh);
}

void steam__column_default(
  struct steam_column *const column
) {
  for (int lvl = 0; lvl < STEAM__LVL_COUNT; lvl++) {
    column->ring_offsets[lvl] = (struct vec3){0, lvl * LVL_HEIGHT, 0};
    column->ring_radii[lvl] = MIN_RING_RADII;
  }
  for (int i = 0; i < 300; i++)
    steam__rise(
      0.1f, // fake delta time,
      column
    );
}

void steam__shared_init_mesh_data() {

  static uint_fast16_t acc_index_offset; acc_index_offset = 0;

  for (int lvl = 0; lvl < STEAM__LVL_COUNT; lvl++) {

    static uint_fast16_t lvl_starting_vert;
    lvl_starting_vert = lvl * VERTS_PER_LVL;

    for (int vert_offset = 0; vert_offset < VERTS_PER_LVL-1; vert_offset++) {

      static uint_fast16_t acc_vi;
      acc_vi = lvl_starting_vert + vert_offset;

      shared_column_mesh.indices[acc_index_offset++] = acc_vi;
      shared_column_mesh.indices[acc_index_offset++] = acc_vi + 1;
      shared_column_mesh.indices[acc_index_offset++] =
        acc_vi + VERTS_PER_LVL + 1;

      shared_column_mesh.indices[acc_index_offset++] = acc_vi;
      shared_column_mesh.indices[acc_index_offset++] =
        acc_vi + VERTS_PER_LVL + 1;
      shared_column_mesh.indices[acc_index_offset++] =
        acc_vi + VERTS_PER_LVL;
    }

    shared_column_mesh.indices[acc_index_offset++] =
      lvl_starting_vert + VERTS_PER_LVL - 1;
    shared_column_mesh.indices[acc_index_offset++] =
      lvl_starting_vert;
    shared_column_mesh.indices[acc_index_offset++] =
      lvl_starting_vert + VERTS_PER_LVL;

    shared_column_mesh.indices[acc_index_offset++] =
      lvl_starting_vert + VERTS_PER_LVL - 1;
    shared_column_mesh.indices[acc_index_offset++] =
      lvl_starting_vert + VERTS_PER_LVL;
    shared_column_mesh.indices[acc_index_offset++] =
      lvl_starting_vert + VERTS_PER_LVL * 2 - 1;
  }
}

static void calculate_ring_vertex_positions(
  uint16_t lvl,
  struct vec3 ring_offset,
  float ring_radius
) {
  
  static struct m4x4 rotation;

  for (int vert_offset = 0; vert_offset < VERTS_PER_LVL; vert_offset++) {

    static struct vec3 position; position = (struct vec3){0};
    position.z = -ring_radius;

    m4x4__rotation(
      deg_to_rad(vert_offset * RING_VERT_DEG_OFFSET),
      WORLDSPACE.up,
      &rotation
    );
    position = m4x4_x_point(&rotation, position);

    shared_column_mesh.vertices[
      lvl * VERTS_PER_LVL + vert_offset
    ].position = vec3_plus_vec3(position, ring_offset);
  }
}

static struct vec3 calculate_face_normal(
  uint_fast16_t face_start_index
) {
  return vec3__cross(
    vec3__normalize(vec3_minus_vec3(
      shared_column_mesh.vertices[
        shared_column_mesh.indices[face_start_index]
      ].position,
      shared_column_mesh.vertices[
        shared_column_mesh.indices[face_start_index + 1]
      ].position
    )),
    vec3__normalize(vec3_minus_vec3(
      shared_column_mesh.vertices[
        shared_column_mesh.indices[face_start_index + 1]
      ].position,
      shared_column_mesh.vertices[
        shared_column_mesh.indices[face_start_index + 2]
      ].position
    ))
  );
}

static inline uint_fast16_t vert_i_to_face_i(
  uint_fast16_t vi
) { return vi * 6; }

static inline uint_fast16_t wrap_lvl_vi_ccw(
  uint_fast16_t lvl,
  uint_fast16_t vi 
) {
  return
    vi == (lvl + 1) * VERTS_PER_LVL ?
    lvl * VERTS_PER_LVL :
    vi;
}

static inline uint_fast16_t wrap_lvl_vi_cw(
  uint_fast16_t lvl,
  uint_fast16_t vi
) {
  return
    (vi == -1 || lvl * VERTS_PER_LVL - 1) ?
    (lvl + 1) * VERTS_PER_LVL - 1 :
    vi;
}

static void calculate_column_normals(
  struct steam_column const *const column
) {
  for (uint_fast8_t lvl = 0; lvl < STEAM__LVL_COUNT - 1; lvl++) {

    static uint_fast16_t lvl_starting_vi;
    lvl_starting_vi = lvl * VERTS_PER_LVL;

    for (uint_fast8_t vi = 0; vi < VERTS_PER_LVL; vi++) {

      static uint_fast16_t acc_vi;
      acc_vi = lvl_starting_vi + vi;
      
      shared_column_mesh.vertices[acc_vi].normal =
        calculate_face_normal(vert_i_to_face_i(acc_vi));
        // TODO: defo the most expensive thing we're doing for the background
        // may want to profile
    }
  }
}

void steam__rise(
  double delta_time,
  struct steam_column *const column
) {

  static float shape[STEAM__LVL_COUNT] = {
    1, 1.1f, 1.2f, 1.2f, 1.1f,
    1.2f, 1.3f, 1.3f, 1.2f, 1.2f,
    1.1f, 1.1f, 1.2f, 1.2f, 1.1f
  };

  for (int_fast8_t lvl = 0; lvl < STEAM__LVL_COUNT; lvl++) {
    column->ring_radii[lvl] +=
      shape[
        (column->shape_index_offset + lvl) %
        STEAM__LVL_COUNT
      ] *
      lvl * lvl * delta_time * 0.01f;

    column->ring_offsets[lvl].y += BOUYANCY * delta_time * 0.2f;
    column->ring_offsets[lvl].x += lvl * 0.05f * delta_time;
  }

  if (
    column->ring_offsets[STEAM__LVL_COUNT - 1].y >
    MAX_COLUMN_HEIGHT
  ) {
    for (int_fast8_t lvl = STEAM__LVL_COUNT - 1; lvl > 0; lvl--) {
      column->ring_offsets[lvl] = column->ring_offsets[lvl - 1];
      column->ring_radii[lvl] = column->ring_radii[lvl - 1];  
    }
    column->ring_radii[0] = MIN_RING_RADII;
    column->ring_offsets[0] = (struct vec3){0};
    column->shape_index_offset =
      column->shape_index_offset - 1 < 0 ?
      STEAM__LVL_COUNT :
      column->shape_index_offset - 1;
  }
}

void steam__draw_column(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct steam_column *const column
) {
  static struct m4x4 local_to_world;
  static struct m3x3 normals_local_to_world;

  for (int lvl = 0; lvl < STEAM__LVL_COUNT; lvl++) {
    calculate_ring_vertex_positions(
      lvl,
      column->ring_offsets[lvl],
      column->ring_radii[lvl]
    );
  }
  calculate_column_normals(column);
  gpu->update_gpu_mesh_data(&shared_column_mesh);
  
  gpu->select_shader(&shared_steam_shader);
  gpu->set_fragment_shader_vec3(
    &shared_steam_shader,
    "light_dir",
    vec3__normalize((struct vec3){ 1.0f, 0, -0.5f })
  );
  gpu->set_fragment_shader_vec3(
    &shared_steam_shader,
    "top_color",
    COLOR_EVENING_SUNLIGHT
  );
  gpu->set_fragment_shader_float(
    &shared_steam_shader,
    "max_altitude",
    (LVL_HEIGHT * STEAM__LVL_COUNT) / 2.0f
  );
  m4x4__translation(&column->position, &local_to_world);
  space__create_normals_model(&local_to_world, &normals_local_to_world);
  gpu__set_mvp(
    &local_to_world,
    &normals_local_to_world,
    cam,
    &shared_steam_shader,
    gpu
  );
  gpu->draw_mesh(&shared_column_mesh);
}
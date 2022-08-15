#include <stdio.h>

#include <stdint.h>
#include <math.h>

#include "tail.h"
#include "steam.h"

#include "gpu_helpers.h"
#include "constants.h"
#include "bull_math.h"

#include "default_vert.h"
#include "normal_debug_frag.h"
#include "steam_frag.h"

#define VERTS_PER_LVL 6
#define RING_VERT_DEG_OFFSET 360.0f / VERTS_PER_LVL
#define STEAM__VERT_COUNT VERTS_PER_LVL * STEAM__COLUMN_LVL_COUNT 
#define STEAM__INDEX_COUNT VERTS_PER_LVL * 6 * (STEAM__COLUMN_LVL_COUNT - 1)

static struct shader shared_steam_shader;

static struct drawable_mesh shared_column_mesh = (struct drawable_mesh){
  .vertices = (struct vertex[STEAM__VERT_COUNT]){0},
  .indices = (unsigned int[STEAM__INDEX_COUNT]){0},
  .vertices_size = sizeof(struct vertex) * STEAM__VERT_COUNT,
  .indices_size = sizeof(unsigned int) * STEAM__INDEX_COUNT,
  .indices_length = STEAM__INDEX_COUNT
};

void steam__copy_assets_to_gpu(
  struct gpu_api const *const gpu
) {
  // shared_steam_shader.frag_shader_src = steam_frag_src;
  shared_steam_shader.frag_shader_src = normal_debug_frag_src;
  shared_steam_shader.vert_shader_src = default_vert_src;
  gpu->copy_shader_to_gpu(&shared_steam_shader);
  gpu->copy_dynamic_mesh_to_gpu(&shared_column_mesh);
}

void steam__column_default(
  struct steam_column *const column
) {
  for (int lvl = 0; lvl < STEAM__COLUMN_LVL_COUNT; lvl++) {
    column->ring_offsets[lvl] = (struct vec3){0, lvl * 0.5f, 0};
    column->ring_radii[lvl] = 0.5f + lvl * 0.5f;
  }
}

void steam__init_mesh_data() {

  static uint_fast16_t acc_index_offset; acc_index_offset = 0;

  for (int lvl = 0; lvl < STEAM__COLUMN_LVL_COUNT; lvl++) {

    static uint_fast16_t lvl_starting_vert;
    lvl_starting_vert = lvl * VERTS_PER_LVL;

    for (int vert_offset = 0; vert_offset < VERTS_PER_LVL-1; vert_offset++) {

      static uint_fast16_t acc_vi;
      acc_vi = lvl_starting_vert + vert_offset;

      shared_column_mesh.indices[acc_index_offset++] = acc_vi;
      shared_column_mesh.indices[acc_index_offset++] =
        acc_vi + VERTS_PER_LVL + 1;
      shared_column_mesh.indices[acc_index_offset++] = acc_vi + 1;

      shared_column_mesh.indices[acc_index_offset++] = acc_vi;
      shared_column_mesh.indices[acc_index_offset++] =
        acc_vi + VERTS_PER_LVL;
      shared_column_mesh.indices[acc_index_offset++] =
        acc_vi + VERTS_PER_LVL + 1;
    }

    shared_column_mesh.indices[acc_index_offset++] =
      lvl_starting_vert + VERTS_PER_LVL - 1;
    shared_column_mesh.indices[acc_index_offset++] =
      lvl_starting_vert + VERTS_PER_LVL;
    shared_column_mesh.indices[acc_index_offset++] =
      lvl_starting_vert;

    shared_column_mesh.indices[acc_index_offset++] =
      lvl_starting_vert + VERTS_PER_LVL - 1;
    shared_column_mesh.indices[acc_index_offset++] =
      lvl_starting_vert + VERTS_PER_LVL * 2 - 1;
    shared_column_mesh.indices[acc_index_offset++] =
      lvl_starting_vert + VERTS_PER_LVL;
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

static void calculate_column_normals(
  struct steam_column const *const column
) {

  // lvl 0
  for (int vert_offset = 0; vert_offset < VERTS_PER_LVL; vert_offset++)
    shared_column_mesh.vertices[vert_offset].normal =
      vec3__normalize(vec3_minus_vec3(
        shared_column_mesh.vertices[vert_offset].position,
        column->ring_offsets[0]
      ));

  for (int lvl = 1; lvl < STEAM__COLUMN_LVL_COUNT - 1; lvl++) {

    static uint_fast16_t lvl_starting_vert;
    lvl_starting_vert = lvl * VERTS_PER_LVL;

    for (int vert_offset = 0; vert_offset < VERTS_PER_LVL-1; vert_offset++) {
      
      static uint_fast16_t acc_vi;
      acc_vi = lvl_starting_vert + vert_offset;
      static struct vec3 position;
      position = shared_column_mesh.vertices[acc_vi].position;

      static struct vec3 adjacent_edges[4];

      adjacent_edges[0] = vec3_minus_vec3(
        shared_column_mesh.vertices[acc_vi + VERTS_PER_LVL].position,
        position
      );
      adjacent_edges[1] = vec3_minus_vec3(
        shared_column_mesh.vertices[acc_vi - VERTS_PER_LVL].position,
        position
      );

      static int_fast16_t resolved_vi;
      resolved_vi = acc_vi - 1;
      if (resolved_vi < 0) resolved_vi = acc_vi + VERTS_PER_LVL - 1;
      adjacent_edges[2] = vec3_minus_vec3(
        shared_column_mesh.vertices[resolved_vi].position,
        position
      );

      resolved_vi = acc_vi + 1;
      if (resolved_vi == VERTS_PER_LVL) resolved_vi = lvl_starting_vert;
      adjacent_edges[3] = vec3_minus_vec3(
        shared_column_mesh.vertices[resolved_vi].position,
        position
      );

      shared_column_mesh.vertices[acc_vi].normal =
        vec3__normalize(vec3__mean(adjacent_edges, 4));
    }
  }
}

void steam__rise(
  double delta_time,
  double seconds_since_creation,
  struct steam_column *const column
) {

}

void steam__draw_column(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct vec3 light_direction,
  struct steam_column *const column
) {
  static struct m4x4 local_to_world;
  static struct m3x3 normals_local_to_world;

  for (int lvl = 0; lvl < STEAM__COLUMN_LVL_COUNT; lvl++) {
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
    "light_direction",
    light_direction
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
  // gpu->draw_mesh(&shared_column_mesh);
  gpu->draw_wireframe(&shared_column_mesh);
}
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

static struct shader shared_steam_shader;

static struct drawable_mesh shared_column_mesh = (struct drawable_mesh){
  .vertices = (struct vertex[STEAM__VERT_COUNT]){0},
  .indices = (unsigned int[STEAM__INDEX_COUNT]){0},
  .vertices_size = sizeof(struct vertex) * STEAM__VERT_COUNT,
  .indices_size = sizeof(unsigned int) * STEAM__INDEX_COUNT,
  .indices_length = STEAM__INDEX_COUNT
};

static void calculate_ring_vertex_positions(
  uint16_t mesh_vertex_data_offset,
  struct vec3 ring_offset,
  float ring_radius
) {
  static struct vec3 position; position = (vec3){0};
  static struct m4x4 rotation;

  for (int vi = 0; i < STEAM__VERTS_PER_LEVEL; vi++) {

    position.z = -ring_radius;

    m4x4__rotation(
      deg_to_rad(vi * STEAM__LEVEL_VERT_DEG_OFFSET),
      WORLDSPACE.up,
      &rotation
    );
    position = m4x4_x_point(&rotation, position);

    shared_column_mesh.vertices[mesh_vertex_data_offset + vi].position =
      vec3_plus_vec3(position, ring_offset);
  }
}

static void calculate_column_normals(
  struct steam_column const *const column
) {

  for (int i = 0; i < STEAM__VERTS_PER_LEVEL; i++)
    shared_column_mesh.vertices[i].normal = vec3__normalize(vec3_minus_vec3(
      shared_column_mesh.vertices[i].position,
      column->position
    ));

  for (int level = 1; level < STEAM__LEVEL_COUNT - 1; level++) {



    for (int vi = ; vi < STEAM__VERTS_PER_LEVEL - 1; vi++) {
      static vec3 vert_pos_above, vert_pos_below;
      vert_pos_above = vec3_plus_vec3(
        shared_column_mesh.vertices[vi + STEAM__VERTS_PER_LEVEL],
        column->ring_offsets[level]
      );
      vert_pos_below = vec3_plus_vec(

      );
      static vec3 above_slope, under_slope;
      above_slope = vec3__normalize(vec3_minus_vec3(
        shared_column_mesh.vertices[vi],
        
      ));
    }
  }
}

void steam__init_mesh_data() {

  for (int level = 0; level < STEAM__LEVEL_COUNT - 1; level++) {
    
    static struct vec3 ring_offset;
    ring_offset.y = level * STEAM__LEVEL_VERTICAL_OFFSET;

    calculate_ring_vertex_positions(
      level * STEAM__VERTS_PER_LEVEL,
      ring_offset,
      level
    );

    static uint_fast16_t vert_index_offset;
    vert_index_offset = 0;
    static uint_fast16_t level_starting_vert;
    level_starting_vert = level * STEAM__VERTS_PER_LEVEL;

    // we want STEAM__VERTS_PER_LEVEL - 1 since we're looping the mesh here
    // the final faces will have indices from the start and end of the ring
    for (int v = 0; v < STEAM__VERTS_PER_LEVEL - 1; v++) {

      static uint_fast16_t current_vert;
      current_vert = level_starting_vert + v;

      // first face
      shared_column_mesh.indices[vert_index_offset++] = current_vert;
      shared_column_mesh.indices[vert_index_offset++] =
        current_vert + STEAM__VERTS_PER_LEVEL + 1;
      shared_column_mesh.indices[vert_index_offset++] = current_vert + 1;

      // second face
      shared_column_mesh.indices[vert_index_offset++] = current_vert;
      shared_column_mesh.indices[vert_index_offset++] =
        current_vert + STEAM__VERTS_PER_LEVEL;
      shared_column_mesh.indices[vert_index_offset++] =
        current_vert + STEAM__VERTS_PER_LEVEL + 1;
    }

    // first face
    shared_column_mesh.indices[vert_index_offset++] =
      level_starting_vert + STEAM__VERTS_PER_LEVEL - 1;
    shared_column_mesh.indices[vert_index_offset++] =
      level_starting_vert + STEAM__VERTS_PER_LEVEL;
    shared_column_mesh.indices[vert_index_offset++] =
      level_starting_vert;

    // second face
    shared_column_mesh.indices[vert_index_offset++] =
      level_starting_vert + STEAM__VERTS_PER_LEVEL - 1;
    shared_column_mesh.indices[vert_index_offset++] =
      (level + 1) * STEAM__VERTS_PER_LEVEL - 1;
    shared_column_mesh.indices[vert_index_offset++] =
      level_starting_vert + STEAM__VERTS_PER_LEVEL;
  }
}

void steam__column_default(
  struct steam_column *const column
) {

}

void steam__copy_assets_to_gpu(
  struct gpu_api const *const gpu
) {
  shared_steam_shader.frag_shader_src = steam_frag_src;
  shared_steam_shader.vert_shader_src = default_vert_src;
  gpu->copy_shader_to_gpu(&shared_steam_shader);
  gpu->copy_dynamic_mesh_to_gpu(&shared_column_mesh);
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
  struct steam_column *const column
) {
  static struct m4x4 local_to_world;
  static struct m3x3 normals_local_to_world;

  for (int level = 0; level < STEAM__LEVEL_COUNT - 1; level++) {
    calculate_ring_vertex_positions(
      level * STEAM__VERTS_PER_LEVEL,
      column->ring_offsets[level],
      column->ring_radii[level]
    );
  }
  
  gpu->select_shader(&shared_steam_shader);
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
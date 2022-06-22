#include <string.h>
#include <stdio.h>
#include <math.h>

#include "tail.h"

#include "menu_sky.h"
#include "gpu_helpers.h"
#include "constants.h"

#include "sky_frag.h"
#include "default_vert.h"
#include "normal_debug_frag.h"
#include "solid_color_frag.h"

#define VERTS_PER_SIDE 41
#define INDEX_COUNT 9600
#define SQUARE_FACE_WIDTH 0.05f

static struct gpu_program shader;
static struct m4x4 local_to_world;
static struct m3x3 normals_local_to_world;
static struct transform trans = {
  .position = { -1, -1, 0 },
  .rotation_in_deg = { 0, 25, 0 },
  .scale = 1
};

static struct vertex vertices[VERTS_PER_SIDE * VERTS_PER_SIDE];
static unsigned int indices[INDEX_COUNT];
static struct drawable_mesh mesh = {
  .vertex_buffer = vertices,
  .index_buffer = indices,
  .vertex_buffer_size = sizeof(vertices),
  .index_buffer_size = sizeof(indices),
  .index_buffer_length = INDEX_COUNT
};

static void warp_mesh(double seconds_since_creation);
static void recalculate_normals();

void menu_sky__init(const struct gpu_api *gpu) {

  // VERTEX BUFFER
  int vert_index = 0;
  for (int y = 0; y < VERTS_PER_SIDE; y++) {
    for (int x = 0; x < VERTS_PER_SIDE; x++) {
      mesh.vertex_buffer[vert_index].position.x = x * SQUARE_FACE_WIDTH;
      mesh.vertex_buffer[vert_index].position.y = y * SQUARE_FACE_WIDTH;
      mesh.vertex_buffer[vert_index].uv.x =
        x * SQUARE_FACE_WIDTH / SQUARE_FACE_WIDTH * VERTS_PER_SIDE;
      mesh.vertex_buffer[vert_index].uv.y =
        y * SQUARE_FACE_WIDTH / SQUARE_FACE_WIDTH * VERTS_PER_SIDE;
      vert_index++;
    }
  }

  // INDEX BUFFER
  vert_index = -1;
  int indices_index = 0;
  int column_index = 0;
  while (vert_index++ < VERTS_PER_SIDE * (VERTS_PER_SIDE - 1) - 1) {
    column_index = (vert_index + 1) % VERTS_PER_SIDE;
    if (vert_index > 0 && column_index == 0) continue;
    mesh.index_buffer[indices_index++] = vert_index;
    mesh.index_buffer[indices_index++] = vert_index + 1;
    mesh.index_buffer[indices_index++] = vert_index + VERTS_PER_SIDE + 1;
    mesh.index_buffer[indices_index++] = vert_index;
    mesh.index_buffer[indices_index++] = vert_index + VERTS_PER_SIDE + 1;
    mesh.index_buffer[indices_index++] = vert_index + VERTS_PER_SIDE;
  }

  shader.frag_shader_src = normal_debug_frag_src;
  shader.vert_shader_src = default_vert_src;

  gpu->copy_dynamic_mesh_to_gpu(&mesh);
  gpu->copy_program_to_gpu(&shader);
}

void menu_sky__tick(
  double delta_time,
  double seconds_since_creation,
  const struct gpu_api *gpu
) {
  warp_mesh(seconds_since_creation);
  gpu->update_gpu_mesh_data(&mesh);
  recalculate_normals();
}

void menu_sky__draw(
  const struct gpu_api *gpu,
  const struct m4x4 *view,
  const struct m4x4 *perspective
) {
  gpu->cull_back_faces();
  gpu->select_gpu_program(&shader);
  space__create_model(&WORLDSPACE, &trans, &local_to_world);
  space__create_normals_model(&local_to_world, &normals_local_to_world);
  gpu__set_mvp(
    &local_to_world,
    view,
    perspective,
    &normals_local_to_world,
    &shader,
    gpu
  );
  gpu->set_fragment_shader_vec3(&shader, "color", &COLOR_BLACK);
  gpu->draw_mesh(&mesh);
}

static void warp_mesh(double seconds_since_creation) {
  int vert_index = 0;
  float z_position = 0;
  for (int y = 0; y < VERTS_PER_SIDE; y++) {
    z_position = 0.1f * sin(
      seconds_since_creation * 2 +
      10 * y * SQUARE_FACE_WIDTH
    );
    for (int x = 0; x < VERTS_PER_SIDE; x++)
      mesh.vertex_buffer[vert_index++].position.z = z_position;
  }
}

static void recalculate_normals() {

  int vert_index = 0;
  struct vec3 x_edge = { 1, 0, 0 };
  struct vec3 y_edges[2] = {0};
  struct vec3 final_y_edge = {0};
  struct vec3 normal = {0};

  for (int y = 0; y < VERTS_PER_SIDE; y++) {

    if (y == 0) {
      vec3_minus_vec3(
        &mesh.vertex_buffer[vert_index].position,
        &mesh.vertex_buffer[vert_index + VERTS_PER_SIDE].position,
        &final_y_edge
      );
    } else if (y == VERTS_PER_SIDE - 1) {
      vec3_minus_vec3(
        &mesh.vertex_buffer[vert_index - VERTS_PER_SIDE].position,
        &mesh.vertex_buffer[vert_index].position,
        &final_y_edge
      );
    } else {
      vec3_minus_vec3(
        &mesh.vertex_buffer[vert_index].position,
        &mesh.vertex_buffer[vert_index + VERTS_PER_SIDE].position,
        &y_edges[0]
      );
      vec3_minus_vec3(
        &mesh.vertex_buffer[vert_index - VERTS_PER_SIDE].position,
        &mesh.vertex_buffer[vert_index].position,
        &y_edges[1]
      );
      vec3__mean(y_edges, 2, &final_y_edge);
    }

    vec3__cross(&final_y_edge, &x_edge, &normal);
    vec3__normalize(&normal, &normal);

    for (int x = 0; x < VERTS_PER_SIDE; x++) memcpy(
      &mesh.vertex_buffer[vert_index++].normal.x,
      &normal.x,
      sizeof(struct vec3)
    );
  }
}
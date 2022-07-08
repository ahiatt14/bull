#include <string.h>
#include <stdio.h>
#include <math.h>

#include "tail.h"

#include "menu_sky.h"
#include "gpu_helpers.h"
#include "constants.h"

#include "clod256_texture.h"
#include "flat_texture_frag.h"
#include "default_vert.h"
#include "normal_debug_frag.h"
#include "solid_color_frag.h"

#define VERTS_PER_SIDE 41
#define INDEX_COUNT 9600
#define SQUARE_FACE_WIDTH 0.05f

static struct shader shad;
static struct m4x4 local_to_world;
static struct m3x3 normals_local_to_world;
static struct transform trans = {
  .position = { 0, 0, 0 },
  .rotation_in_deg = { 0, 0, 0 },
  .scale = 2
};

static struct vertex vertices[VERTS_PER_SIDE * VERTS_PER_SIDE];
static unsigned int indices[INDEX_COUNT];
static struct drawable_mesh mesh = {
  .vertices = vertices,
  .indices = indices,
  .vertices_size = sizeof(struct vertex) * VERTS_PER_SIDE * VERTS_PER_SIDE,
  .indices_size = sizeof(unsigned int) * INDEX_COUNT,
  .indices_length = INDEX_COUNT
};

static const struct vec3 ORIGIN_OFFSET = {
  -SQUARE_FACE_WIDTH * VERTS_PER_SIDE / 2.0f,
  -SQUARE_FACE_WIDTH * VERTS_PER_SIDE / 2.0f,
  0
};

static void warp_mesh(double seconds_since_creation);
static void recalculate_normals();

void menu_sky__init(const struct gpu_api *gpu) {

  // VERTEX BUFFER
  int vert_index = 0;
  for (int y = 0; y < VERTS_PER_SIDE; y++) {
    for (int x = 0; x < VERTS_PER_SIDE; x++) {
      mesh.vertices[vert_index].position.x =
        ORIGIN_OFFSET.x + x * SQUARE_FACE_WIDTH;
      mesh.vertices[vert_index].position.y =
        ORIGIN_OFFSET.y + y * SQUARE_FACE_WIDTH;
      mesh.vertices[vert_index].uv.x =
        x * SQUARE_FACE_WIDTH / SQUARE_FACE_WIDTH * VERTS_PER_SIDE;
      mesh.vertices[vert_index].uv.y =
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
    mesh.indices[indices_index++] = vert_index;
    mesh.indices[indices_index++] = vert_index + 1;
    mesh.indices[indices_index++] = vert_index + VERTS_PER_SIDE + 1;
    mesh.indices[indices_index++] = vert_index;
    mesh.indices[indices_index++] = vert_index + VERTS_PER_SIDE + 1;
    mesh.indices[indices_index++] = vert_index + VERTS_PER_SIDE;
  }

  // shad.frag_shad_src = normal_debug_frag_src;
  shad.frag_shader_src = flat_texture_frag_src;
  shad.vert_shader_src = default_vert_src;

  gpu->copy_rgb_texture_to_gpu(&clod256_texture);

  gpu->copy_dynamic_mesh_to_gpu(&mesh);
  gpu->copy_shader_to_gpu(&shad);
}

void menu_sky__tick(
  double delta_time,
  double seconds_since_creation,
  const struct gpu_api *gpu
) {

  // TODO: update transform in Tail to refer to axes
  // as up, right, and forward
  trans.rotation_in_deg.y -= 2.0f * delta_time;
  warp_mesh(seconds_since_creation);
  recalculate_normals();
  gpu->update_gpu_mesh_data(&mesh);
}

void menu_sky__draw(
  const struct gpu_api *gpu,
  const struct camera *cam
) {
  gpu->cull_back_faces();
  gpu->select_shader(&shad);
  gpu->select_texture(&clod256_texture);
  space__create_model(&WORLDSPACE, &trans, &local_to_world);
  space__create_normals_model(&local_to_world, &normals_local_to_world);
  gpu__set_mvp(
    &local_to_world,
    &normals_local_to_world,
    cam,
    &shad,
    gpu
  );
  gpu->draw_mesh(&mesh);
}

static void warp_mesh(double seconds_since_creation) {
  int vert_index = 0;
  float z_position = 0;
  for (int y = 0; y < VERTS_PER_SIDE; y++) {
    z_position = ORIGIN_OFFSET.z + 0.05f * sin(
      seconds_since_creation +
      15 * y * SQUARE_FACE_WIDTH
    );
    for (int x = 0; x < VERTS_PER_SIDE; x++)
      mesh.vertices[vert_index++].position.z = z_position;
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
        &mesh.vertices[vert_index].position,
        &mesh.vertices[vert_index + VERTS_PER_SIDE].position,
        &final_y_edge
      );
    } else if (y == VERTS_PER_SIDE - 1) {
      vec3_minus_vec3(
        &mesh.vertices[vert_index - VERTS_PER_SIDE].position,
        &mesh.vertices[vert_index].position,
        &final_y_edge
      );
    } else {
      vec3_minus_vec3(
        &mesh.vertices[vert_index].position,
        &mesh.vertices[vert_index + VERTS_PER_SIDE].position,
        &y_edges[0]
      );
      vec3_minus_vec3(
        &mesh.vertices[vert_index - VERTS_PER_SIDE].position,
        &mesh.vertices[vert_index].position,
        &y_edges[1]
      );
      vec3__mean(y_edges, 2, &final_y_edge);
    }

    vec3__cross(&final_y_edge, &x_edge, &normal);
    vec3__normalize(&normal, &normal);

    for (int x = 0; x < VERTS_PER_SIDE; x++) memcpy(
      &mesh.vertices[vert_index++].normal.x,
      &normal.x,
      sizeof(struct vec3)
    );
  }
}
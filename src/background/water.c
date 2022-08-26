#include <math.h>

#include "tail.h"

#include "water.h"

#include "constants.h"
#include "bull_math.h"
#include "gpu_helpers.h"

#include "cloud_cover_texture.h"
#include "water_texture.h"

#include "default_vert.h"
#include "water_surface_frag.h"
#include "normal_debug_frag.h"

// CONSTANTS

#define VERTS_PER_SIDE 41
#define FACE_WIDE_KM_LENGTH OCEAN_KM_WIDE / VERTS_PER_SIDE
#define INDEX_COUNT 9600

#define WAVE_AMPLITUDE 0.05f
#define WAVE_FREQUENCY 4.0f

// LOCALS

static struct transform trans = (struct transform){
  {0, -0.1f, 0},
  {270, 0, 0},
  1
};

static struct vertex vertices[VERTS_PER_SIDE * VERTS_PER_SIDE];
static unsigned int indices[INDEX_COUNT];
static struct drawable_mesh mesh = {
  .vertices = vertices,
  .indices = indices,
  .vertices_size = sizeof(struct vertex) *
    VERTS_PER_SIDE * VERTS_PER_SIDE,
  .indices_size = sizeof(unsigned int) * INDEX_COUNT,
  .indices_length = INDEX_COUNT
};

static const struct vec3 ORIGIN_OFFSET = {
  -FACE_WIDE_KM_LENGTH * VERTS_PER_SIDE * 0.5f,
  -FACE_WIDE_KM_LENGTH * VERTS_PER_SIDE * 0.5f,
  0
};

static struct shader surface_shader;

static struct m4x4 shared_local_to_world;
static struct m3x3 shared_normals_local_to_world;

void water__init_mesh_data() {

  // VERTEX BUFFER
  int vert_index = 0;
  for (int y = 0; y < VERTS_PER_SIDE; y++) {
    for (int x = 0; x < VERTS_PER_SIDE; x++) {
      mesh.vertices[vert_index].position.x =
        ORIGIN_OFFSET.x + x * FACE_WIDE_KM_LENGTH;
      mesh.vertices[vert_index].position.y =
        ORIGIN_OFFSET.y + y * FACE_WIDE_KM_LENGTH;
      mesh.vertices[vert_index].uv.x = x *
        FACE_WIDE_KM_LENGTH / (FACE_WIDE_KM_LENGTH *
        VERTS_PER_SIDE);
      mesh.vertices[vert_index].uv.y = y *
        FACE_WIDE_KM_LENGTH / (FACE_WIDE_KM_LENGTH *
        VERTS_PER_SIDE);
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
}

void water__copy_assets_to_gpu(
  struct gpu_api const *const gpu
) {
  surface_shader.vert_shader_src = default_vert_src;
  surface_shader.frag_shader_src = water_surface_frag_src;
  gpu->copy_dynamic_mesh_to_gpu(&mesh);
  gpu->copy_shader_to_gpu(&surface_shader);
  gpu->copy_rgb_texture_to_gpu(&water_texture);
  gpu->copy_rgb_texture_to_gpu(&cloud_cover_texture);
}

void water__update_waves(
  struct vec2 wind_km_per_sec,
  double delta_time,
  double seconds_since_creation,
  struct gpu_api const *const gpu
) {

  // UPDATE WAVE VERTEX DATA

  // TODO: cache these?
  int vert_index = 0;
  float z_position = 0;
  for (int y = 0; y < VERTS_PER_SIDE; y++) {
    z_position = WAVE_AMPLITUDE * sin(
      seconds_since_creation + WAVE_FREQUENCY * y
    );
    for (int x = 0; x < VERTS_PER_SIDE; x++) {
      mesh.vertices[vert_index].uv.x += wind_km_per_sec.x * delta_time;
      mesh.vertices[vert_index].uv.y += wind_km_per_sec.y * delta_time;
      mesh.vertices[vert_index++].position.z = z_position;
    }
  }

  // UPDATE WAVE NORMALS

  // TODO: cache these?
  vert_index = 0;
  struct vec3 x_edge = { 1, 0, 0 };
  struct vec3 y_edges[2] = {0};
  struct vec3 final_y_edge = {0};
  struct vec3 normal = {0};

  for (int y = 0; y < VERTS_PER_SIDE; y++) {

    if (y == 0) {
      final_y_edge = vec3_minus_vec3(
        mesh.vertices[vert_index].position,
        mesh.vertices[vert_index + VERTS_PER_SIDE].position
      );
    } else if (y == VERTS_PER_SIDE - 1) {
      final_y_edge = vec3_minus_vec3(
        mesh.vertices[vert_index - VERTS_PER_SIDE].position,
        mesh.vertices[vert_index].position
      );
    } else {
      y_edges[0] = vec3_minus_vec3(
        mesh.vertices[vert_index].position,
        mesh.vertices[vert_index + VERTS_PER_SIDE].position
      );
      y_edges[1] = vec3_minus_vec3(
        mesh.vertices[vert_index - VERTS_PER_SIDE].position,
        mesh.vertices[vert_index].position
      );
      final_y_edge = vec3__mean(y_edges, 2);
    }

    normal = vec3__normalize(vec3__cross(final_y_edge, x_edge));

    for (int x = 0; x < VERTS_PER_SIDE; x++)
      mesh.vertices[vert_index++].normal = normal;
  }

  gpu->update_gpu_mesh_data(&mesh);
}

void water__draw(
  struct vec3 sunlight_direction,
  struct vec3 sunlight_color,
  struct camera const *const cam,
  struct gpu_api const *const gpu
) {
  gpu->select_shader(&surface_shader);
  gpu->select_textures(
    &surface_shader,
    2,
    (char const *const[]){
      "surface_texture",
      "cloud_cover"
    },
    (struct texture const *const[]){
      &water_texture,
      &cloud_cover_texture
    }
  );
  gpu->set_fragment_shader_vec3(
    &surface_shader,
    "light_dir",
    sunlight_direction
  );
  gpu->set_fragment_shader_vec3(
    &surface_shader,
    "light_color",
    sunlight_color
  );
  space__create_model(
    &WORLDSPACE,
    &trans,
    &shared_local_to_world
  );
  space__create_normals_model(
    &shared_local_to_world,
    &shared_normals_local_to_world
  );
  gpu__set_mvp(
    &shared_local_to_world,
    &shared_normals_local_to_world,
    cam,
    &surface_shader,
    gpu
  );
  gpu->draw_mesh(&mesh);
}
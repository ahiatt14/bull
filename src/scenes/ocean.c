#include <math.h>

#include "tail.h"

#include "scene.h"
#include "constants.h"
#include "bull_math.h"
#include "gpu_helpers.h"

#include "default_vert.h"
#include "normal_debug_frag.h"
#include "solid_color_frag.h"

// CONSTANTS

#define OCEAN_VERTS_PER_SIDE 41
#define OCEAN_INDEX_COUNT 9600
#define OCEAN_SQUARE_FACE_WIDTH 0.05f

// FORWARD DECS

static void warp_ocean_mesh(
  double delta_time,
  double seconds_since_creation
);
static void recalculate_ocean_normals();

// LOCALS

static double seconds_since_creation;
static double tick_start_time;
static double delta_time;

static struct camera cam;

static struct vertex vertices[OCEAN_VERTS_PER_SIDE * OCEAN_VERTS_PER_SIDE];
static unsigned int indices[OCEAN_INDEX_COUNT];
static struct drawable_mesh ocean_mesh = {
  .vertices = vertices,
  .indices = indices,
  .vertices_size = sizeof(struct vertex) *
    OCEAN_VERTS_PER_SIDE * OCEAN_VERTS_PER_SIDE,
  .indices_size = sizeof(unsigned int) * OCEAN_INDEX_COUNT,
  .indices_length = OCEAN_INDEX_COUNT
};

static const struct vec3 OCEAN_ORIGIN_OFFSET = {
  -OCEAN_SQUARE_FACE_WIDTH * OCEAN_VERTS_PER_SIDE / 2.0f,
  -OCEAN_SQUARE_FACE_WIDTH * OCEAN_VERTS_PER_SIDE / 2.0f,
  0
};

static struct transform ocean_transform = (struct transform){
  {0, 0, 0},
  {270, 0, 0},
  2
};
static struct shader water_surface_shader;
static struct m4x4 shared_local_to_world;
static struct m3x3 shared_normals_local_to_world;

void ocean__init(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu
) {

  camera__init(&cam);
  camera__set_position(0, 3, 5, &cam);
  camera__set_look_target(ORIGIN, &cam);
  camera__set_horizontal_fov_in_deg(60, &cam);
  camera__set_near_clip_distance(1, &cam);
  camera__set_far_clip_distance(70, &cam);
  camera__calculate_lookat(WORLDSPACE.up, &cam);
  camera__calculate_perspective(vwprt, &cam);

  // OCEAN VERTEX BUFFER
  int vert_index = 0;
  for (int y = 0; y < OCEAN_VERTS_PER_SIDE; y++) {
    for (int x = 0; x < OCEAN_VERTS_PER_SIDE; x++) {
      ocean_mesh.vertices[vert_index].position.x =
        OCEAN_ORIGIN_OFFSET.x + x * OCEAN_SQUARE_FACE_WIDTH;
      ocean_mesh.vertices[vert_index].position.y =
        OCEAN_ORIGIN_OFFSET.y + y * OCEAN_SQUARE_FACE_WIDTH;
      ocean_mesh.vertices[vert_index].uv.x = x *
      OCEAN_SQUARE_FACE_WIDTH / OCEAN_SQUARE_FACE_WIDTH *
      OCEAN_VERTS_PER_SIDE;
      ocean_mesh.vertices[vert_index].uv.y = y *
      OCEAN_SQUARE_FACE_WIDTH / OCEAN_SQUARE_FACE_WIDTH *
      OCEAN_VERTS_PER_SIDE;
      vert_index++;
    }
  }

  // OCEAN INDEX BUFFER
  vert_index = -1;
  int indices_index = 0;
  int column_index = 0;
  while (vert_index++ < OCEAN_VERTS_PER_SIDE * (OCEAN_VERTS_PER_SIDE - 1) - 1) {
    column_index = (vert_index + 1) % OCEAN_VERTS_PER_SIDE;
    if (vert_index > 0 && column_index == 0) continue;
    ocean_mesh.indices[indices_index++] = vert_index;
    ocean_mesh.indices[indices_index++] = vert_index + 1;
    ocean_mesh.indices[indices_index++] = vert_index + OCEAN_VERTS_PER_SIDE + 1;
    ocean_mesh.indices[indices_index++] = vert_index;
    ocean_mesh.indices[indices_index++] = vert_index + OCEAN_VERTS_PER_SIDE + 1;
    ocean_mesh.indices[indices_index++] = vert_index + OCEAN_VERTS_PER_SIDE;
  }

  // shad.frag_shader_src = flat_texture_frag_src;
  water_surface_shader.vert_shader_src = default_vert_src;
  water_surface_shader.frag_shader_src = normal_debug_frag_src;

  // gpu->copy_rgb_texture_to_gpu(&clod256_texture);

  gpu->copy_dynamic_mesh_to_gpu(&ocean_mesh);
  gpu->copy_shader_to_gpu(&water_surface_shader);
}

void ocean__tick(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu,
  uint8_t previous_scene,
  void (*switch_scene)(uint8_t new_scene)
) {

  // GAME TIME

  tick_start_time = seconds_since_creation;
  seconds_since_creation = window->get_seconds_since_creation();
  delta_time = seconds_since_creation - tick_start_time;
  if (delta_time > DELTA_TIME_CAP) delta_time = DELTA_TIME_CAP;

  // UPDATE

  warp_ocean_mesh(
    delta_time,
    seconds_since_creation
  );
  recalculate_ocean_normals();

  // DRAW

  // TODO: temp
  gpu->clear(&COLOR_SKY_BLUE);

  gpu->cull_back_faces();
  gpu->select_shader(&water_surface_shader);
  // gpu->select_texture(&clod256_texture);
  space__create_model(
    &WORLDSPACE,
    &ocean_transform,
    &shared_local_to_world
  );
  space__create_normals_model(
    &shared_local_to_world,
    &shared_normals_local_to_world
  );
  gpu__set_mvp(
    &shared_local_to_world,
    &shared_normals_local_to_world,
    &cam,
    &water_surface_shader,
    gpu
  );
  gpu->draw_mesh(&ocean_mesh);
}

// HELPERS

static void warp_ocean_mesh(
  double delta_time,
  double seconds_since_creation
) {
  int vert_index = 0;
  float z_position = 0;
  for (int y = 0; y < OCEAN_VERTS_PER_SIDE; y++) {
    z_position = OCEAN_ORIGIN_OFFSET.z + 0.05f * sin(
      seconds_since_creation +
      15 * y * OCEAN_SQUARE_FACE_WIDTH
    );
    for (int x = 0; x < OCEAN_VERTS_PER_SIDE; x++)
      ocean_mesh.vertices[vert_index++].position.z = z_position;
  }
}

static void recalculate_ocean_normals() {

  int vert_index = 0;
  struct vec3 x_edge = { 1, 0, 0 };
  struct vec3 y_edges[2] = {0};
  struct vec3 final_y_edge = {0};
  struct vec3 normal = {0};

  for (int y = 0; y < OCEAN_VERTS_PER_SIDE; y++) {

    if (y == 0) {
      final_y_edge = vec3_minus_vec3(
        ocean_mesh.vertices[vert_index].position,
        ocean_mesh.vertices[vert_index + OCEAN_VERTS_PER_SIDE].position
      );
    } else if (y == OCEAN_VERTS_PER_SIDE - 1) {
      final_y_edge = vec3_minus_vec3(
        ocean_mesh.vertices[vert_index - OCEAN_VERTS_PER_SIDE].position,
        ocean_mesh.vertices[vert_index].position
      );
    } else {
      y_edges[0] = vec3_minus_vec3(
        ocean_mesh.vertices[vert_index].position,
        ocean_mesh.vertices[vert_index + OCEAN_VERTS_PER_SIDE].position
      );
      y_edges[1] = vec3_minus_vec3(
        ocean_mesh.vertices[vert_index - OCEAN_VERTS_PER_SIDE].position,
        ocean_mesh.vertices[vert_index].position
      );
      final_y_edge = vec3__mean(y_edges, 2);
    }

    normal = vec3__normalize(vec3__cross(final_y_edge, x_edge));

    for (int x = 0; x < OCEAN_VERTS_PER_SIDE; x++)
      ocean_mesh.vertices[vert_index++].normal = normal;
  }
}
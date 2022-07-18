#include <stdint.h>

#include "tail.h"
#include "arena.h"

#include "gpu_helpers.h"
#include "constants.h"

#include "arena_cage_frag.h"
#include "default_vert.h"
#include "cage_mesh.h"

static struct shader cage_shader;

static struct m4x4 shared_local_to_world;
static struct m3x3 shared_normals_local_to_world;

void arena__copy_assets_to_gpu(
  struct gpu_api const *const gpu
) {
  cage_shader.frag_shader_src = arena_cage_frag_src;
  cage_shader.vert_shader_src = default_vert_src;
  gpu->copy_shader_to_gpu(&cage_shader);
  gpu->copy_static_mesh_to_gpu(&cage_mesh);
}

void arena__update(
  double delta_time,
  struct arena_state *const arena
) {
  arena->transform.rotation_in_deg.x += 2 * delta_time;
  arena->transform.rotation_in_deg.z += 4 * delta_time;
}

void arena__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct arena_state const *const arena
) {
  gpu->cull_back_faces();
  gpu->select_shader(&cage_shader);
  gpu->set_fragment_shader_vec3(
    &cage_shader,
    "hot_color",
    &COLOR_GOLDEN_YELLOW
  );
  space__create_model(
    &WORLDSPACE,
    &arena->transform,
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
    &cage_shader,
    gpu
  );
  gpu->draw_wireframe(&cage_mesh);
}
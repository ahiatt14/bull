#include "tail.h"

#include "ecs.h"

#include "constants.h"

#include "ocean.h"

#include "ocean_surface_mesh.h"
#include "ocean_frag.h"
#include "ocean_vert.h"

static struct Shader shader;

void ocean__copy_assets_to_gpu(
  struct GPU const *const gpu
) {

  shader.vert_src = OCEAN_VERT_SRC;
  shader.frag_src = OCEAN_FRAG_SRC;
  gpu->copy_shader_to_gpu(&shader);

  gpu->copy_static_mesh_to_gpu(&OCEAN_SURFACE_MESH);
}

void create_ocean(
  struct ECS *const ecs
) {
  
  EntityId ocean = ecs__create_entity(ecs);

  ecs__add_transform(
    ocean,
    (struct Transform){
      .position = ORIGIN,
      .scale = 10
    },
    ecs
  );
  ecs__add_draw(
    ocean,
    (struct Draw){
      .shader = &shader,
      .mesh = &OCEAN_SURFACE_MESH,
      .texture = NULL,
      .draw = ecs__draw_mesh
    },
    ecs
  );
}
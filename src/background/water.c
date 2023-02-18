#include "tail.h"

#include "ecs.h"

#include "constants.h"

#include "water.h"

#include "ocean_surface_mesh.h"
#include "ocean_frag.h"
#include "ocean_vert.h"

#include "solid_color_frag.h"

static struct Shader shader;

void water__copy_assets_to_gpu(
  struct GPU const *const gpu
) {

  shader.vert_src = OCEAN_VERT_SRC;
  shader.frag_src = OCEAN_FRAG_SRC;
  gpu->copy_shader_to_gpu(&shader);

  gpu->copy_static_mesh_to_gpu(&OCEAN_SURFACE_MESH);
}

void create_water(
  struct ECS *const ecs
) {
  
  EntityId water = ecs__create_entity(ecs);

  ecs__add_transform(
    water,
    (struct Transform){
      .position = vec3_minus_vec3(
        ORIGIN,
        (struct Vec3){ 0, 1, 0 }
      ),
      .scale = 8
    },
    ecs
  );
  ecs__add_draw(
    water,
    (struct Draw){
      .shader = &shader,
      .mesh = &OCEAN_SURFACE_MESH,
      .texture = NULL,
      .draw = ecs__draw_mesh
    },
    ecs
  );
}
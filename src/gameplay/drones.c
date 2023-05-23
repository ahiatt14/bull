#include "tail.h"

#include "ecs.h"

#include "drones.h"

#include "constants.h"
#include "assets.h"

#include "drone_mesh.h"
#include "standard_material_frag.h"
#include "default_vert.h"

static Shader shader;

void drones__init_scene_callbacks(  
) {

}

void drones__copy_assets_to_gpu(
  GPU const *const gpu 
) {
  shader.frag_src = STANDARD_MATERIAL_FRAG_SRC;
  shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&shader);
  gpu->copy_static_mesh_to_gpu(&DRONE_MESH);
}

EntityId create_drone(
  Vec3 position,
  ECS *const ecs
) {

  EntityId drone = ecs__create_entity(ecs);

  ecs__add_transform(
    drone,
    (Transform){
      .rotation = quaternion__create(WORLDSPACE.up, 0),
      .position = position,
      .scale = 0.5f
    },
    ecs
  );
  ecs__add_receives_light(drone, ecs);
  ecs__add_draw(
    drone,
    (Draw){
      .textures = DARK_RUST_TEXTURE,
      .draw = ecs__draw_mesh,
      .shader = &shader,
      .mesh = &DRONE_MESH
    },
    ecs
  );

  return drone;
}
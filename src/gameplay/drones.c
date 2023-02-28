#include "tail.h"

#include "ecs.h"

#include "drones.h"

#include "constants.h"
#include "assets.h"

// #include "drone_mesh.h"

void drones__init_scene_callbacks(  
) {

}

void drones__copy_assets_to_gpu(
  GPU const *const gpu 
) {
  // gpu->copy_static_mesh_to_gpu(&DRONE_MESH);
}

EntityId create_drone(
  ECS *const ecs
) {

  EntityId drone = ecs__create_entity(ecs);

  ecs__add_transform(
    drone,
    (Transform){
      .scale = 1
    },
    ecs
  );
  ecs__add_look_at_center(drone, ecs);
  ecs__add_alpha_effect(drone, ecs);
  ecs__add_draw(
    drone,
    (Draw){
      // .textures = DRONE_TEXTURE,
      .draw = ecs__draw_mesh,
      .shader = &FLAT_TEXTURE_SHADER,
      // .mesh = &DRONE_MESH
    },
    ecs
  );

  return drone;
}
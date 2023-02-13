#include "tail.h"

#include "ecs.h"

#include "bull_math.h"
#include "constants.h"

#include "mines.h"

#include "lowpoly_sphere_flat_mesh.h"
#include "dark_rust_texture.h"

void mines__copy_assets_to_gpu(
  struct GPU const *const gpu
) {
  gpu->copy_texture_to_gpu(&DARK_RUST_TEXTURE);
  gpu->copy_static_mesh_to_gpu(&LOWPOLY_SPHERE_FLAT_MESH);
}

static void start_revolution(
    EntityId mine,
    Seconds remainder,
    struct ECS *const ecs
) {

}

void create__mine(
  struct Vec3 spawn_start,
  struct Vec3 battlefield_start,
  uint_fast8_t ccw,
  // void (*trigger_detonation)(
  //   EntityId mine,
  //   Seconds remainder,
  //   struct ECS *const ecs
  // ),
  struct ECS *const ecs
) {
   
  EntityId mine = ecs__create_entity(ecs);

  ecs__add_transform(
    mine,
    (struct Transform){
      .scale = 0.25f,
    },
    ecs
  );
  ecs__add_draw(
    mine,
    (struct Draw){
      .texture = &DARK_RUST_TEXTURE,
      .shader = &FLAT_TEXTURE_SHADER,
      .mesh = &LOWPOLY_SPHERE_FLAT_MESH
    },
    ecs
  );
  ecs__add_vec3lerp(
    mine,
    (struct Vec3Lerp){
      .start = spawn_start,
      .end = battlefield_start,
      .age = 0,
      .duration = 3,
      .lerp = vec3__linear_lerp,
      .on_finish = start_revolution
    },
    ecs
  );
}
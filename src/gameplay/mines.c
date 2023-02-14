#include <math.h>

#include "tail.h"

#include "ecs.h"

#include "bull_math.h"
#include "constants.h"

#include "mines.h"

#include "lowpoly_sphere_flat_mesh.h"
#include "dark_rust_texture.h"
#include "blasted_stone_texture.h"

#define MAX_MINES 50

// TODO: gotta be source of the ccw/cw flip bug when mines are destroyed
// before they reach the battlefield
static uint_fast8_t ccw_flags_by_id[MAX_MINES];

static void (*on_shot_ptr)(
  EntityId mine,
  EntityId projectile,
  struct ECS *const ecs
);

static void destroy_mine(
  EntityId id,
  Seconds remainder,
  struct ECS *const ecs
) {
  ecs__mark_for_destruction(id, ecs);
}

void mines__init_scene_callbacks(
  void (*on_shot)(
    EntityId mine,
    EntityId projectile,
    struct ECS *const ecs
  )
) {
  on_shot_ptr = on_shot;
}

void mines__copy_assets_to_gpu(
  struct GPU const *const gpu
) {
  gpu->copy_texture_to_gpu(&BLASTED_STONE_TEXTURE);
  // gpu->copy_texture_to_gpu(&DARK_RUST_TEXTURE);
  gpu->copy_static_mesh_to_gpu(&LOWPOLY_SPHERE_FLAT_MESH);
}

static void start_revolution(
    EntityId mine,
    Seconds remainder,
    struct ECS *const ecs
) {
  ecs__remove_vec3lerp(mine, ecs);
  ecs__add_revolve_lerp(
    mine,
    (struct RevolveLerp){
      .start = ecs->entities[mine].transform.position,
      .target_rads = ccw_flags_by_id[mine] ? M_PI * 10 : -M_PI * 10,
      .age = remainder,
      .duration = 50,
      .on_finish = destroy_mine
    },
    ecs
  );
}

void mines__create_pattern_0(
  struct ECS *const ecs
) {

  struct Vec3 spawn_start = { -2, 0, -10 };
  struct Vec3 spawn_offset = { -2, 0, 0 };

  struct Vec3 battlefield_start = { -2, 0, 0 };
  struct Vec3 battlefield_start_offset = { -2, 0, 0 };

  for (uint_fast8_t i = 0; i < 4; i++) {

    create__mine(
      vec3_plus_vec3(
        spawn_start,
        scalar_x_vec3(i, spawn_offset)
      ),
      vec3_plus_vec3(
        battlefield_start,
        scalar_x_vec3(i, battlefield_start_offset)
      ),
      1,
      ecs
    );
  }
}

void mines__create_pattern_1(
  struct ECS *const ecs
) {

  struct Vec3 spawn_start = { 1, 0, -10 };
  struct Vec3 spawn_offset = { 2, 0, 0 };

  struct Vec3 battlefield_start = { 1, 0, 0 };
  struct Vec3 battlefield_start_offset = { 2, 0, 0 };

  for (uint_fast8_t i = 0; i < 4; i++) {

    create__mine(
      vec3_plus_vec3(
        spawn_start,
        scalar_x_vec3(i, spawn_offset)
      ),
      vec3_plus_vec3(
        battlefield_start,
        scalar_x_vec3(i, battlefield_start_offset)
      ),
      0,
      ecs
    );
  }
}

void create__mine(
  struct Vec3 spawn_start,
  struct Vec3 battlefield_start,
  uint_fast8_t ccw,
  struct ECS *const ecs
) {
   
  EntityId mine = ecs__create_entity(ecs);

  ccw_flags_by_id[mine] = ccw;

  ecs__add_transform(
    mine,
    (struct Transform){
      .scale = 0.33f,
    },
    ecs
  );
  ecs__add_draw(
    mine,
    (struct Draw){
      .texture = &BLASTED_STONE_TEXTURE,
      .shader = &FLAT_TEXTURE_SHADER,
      .mesh = &LOWPOLY_SPHERE_FLAT_MESH,
      .draw = ecs__draw_mesh
    },
    ecs
  );
  ecs__add_vec3lerp(
    mine,
    (struct Vec3Lerp){
      .start = spawn_start,
      .end = battlefield_start,
      .age = 0,
      .duration = 5,
      .lerp = vec3__linear_lerp,
      .on_finish = start_revolution
    },
    ecs
  );
  ecs__add_damagable_radius_collider(
    mine,
    (struct RadiusCollider){
      .radius = 0.2f,
      .on_collide = on_shot_ptr
    },
    ecs
  );
}
#include <math.h>
#include <stdio.h>

#include "tail.h"

#include "ecs.h"

#include "launchers.h"

#include "assets.h"
#include "constants.h"
#include "bull_math.h"

#include "arrow_mesh.h"

#define START_RADIUS 10
#define END_RADIUS 2

static void destroy_launcher(
  EntityId launcher,
  Seconds remainder,
  struct ECS *const ecs
) {
  ecs__mark_for_destruction(launcher, ecs);
}

void (*on_player_collide_with_radial_ptr)(
  EntityId launcher,
  EntityId player,
  struct ECS *const ecs
);

// TODO: cheeky
double (*get_seconds_since_creation_ptr)();

void launchers__copy_assets_to_gpu(
  struct GPU const *const gpu
) {
  gpu->copy_static_mesh_to_gpu(&ARROW_MESH);
}

void launchers__init_scene_callbacks(
  void (*on_player_collide_with_radial)(
    EntityId launcher,
    EntityId player,
    struct ECS *const ecs
  ),
  double (*const get_seconds_since_creation)()
) {
  on_player_collide_with_radial_ptr = on_player_collide_with_radial;
  get_seconds_since_creation_ptr = get_seconds_since_creation;
}

EntityId create_radial_launcher(
  struct Vec3 position,
  struct ECS *const ecs
) {

  EntityId launcher = ecs__create_entity(ecs);

  ecs__add_transform(
    launcher,
    (struct Transform){
      .position = position,
      .scale = 0.7f
    },
    ecs
  );
  ecs__add_look_at_center(launcher, ecs);
  ecs__add_alpha_effect(launcher, ecs);
  ecs__add_draw(
    launcher,
    (struct Draw){
      .textures = ARROW_TEXTURE,
      .draw = ecs__draw_mesh,
      .shader = &FLAT_TEXTURE_SHADER,
      .mesh = &ARROW_MESH
    },
    ecs
  );
  ecs__add_radius_collider(launcher, 0.2f, ecs);
  ecs__add_pickupable(
    launcher,
    on_player_collide_with_radial_ptr,
    ecs
  );
  static struct Vec2 UV_SCROLL_SPEED = (struct Vec2){ 0, -4 };
  ecs__add_uv_scroll(
    launcher,
    (struct ScrollUV){
      .speed = UV_SCROLL_SPEED,
      .total = scalar_x_vec2(
        get_seconds_since_creation_ptr(),
        UV_SCROLL_SPEED
      )
    },
    ecs
  );

  return launcher;
}

static void spawn_radial_launcher(
  EntityId spawner,
  Seconds remainder,
  struct ECS *const ecs
) {

  static float radians;
  radians += M_PI * 0.1f;

  struct Vec3 start = space__ccw_angle_rotate(
    WORLDSPACE.up,
    radians,
    (struct Vec3){ 0, 0, -START_RADIUS }
  );
  struct Vec3 end = space__ccw_angle_rotate(
    WORLDSPACE.up,
    radians,
    (struct Vec3){ 0, 0, -END_RADIUS }
  );

  EntityId launcher = create_radial_launcher(start, ecs);

  ecs__add_vec3lerp(
    launcher,
    (struct Vec3Lerp){
      .start = start,
      .end = end,
      .age = remainder,
      .duration = 7.0, // TODO: base on distance spawned at
      .lerp = vec3__linear_lerp,
      .on_finish = destroy_launcher
    },
    ecs
  );
}

void create_radial_launcher_spawner(
  struct ECS *const ecs
) {

  EntityId spawner = ecs__create_entity(ecs);

  ecs__add_repeat(
    spawner,
    (struct Repeat){
      .age = 0,
      .interval = 0.2f,
      .on_interval = spawn_radial_launcher
    },
    ecs
  );
}
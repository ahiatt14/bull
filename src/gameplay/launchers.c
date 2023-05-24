#include <math.h>
#include <stdio.h>

#include "tail.h"

#include "ecs.h"

#include "launchers.h"

#include "assets.h"
#include "constants.h"
#include "bull_math.h"

#include "afterimages.h"

#define START_RADIUS 5
#define END_RADIUS 2

static void destroy_launcher(
  EntityId launcher,
  Seconds remainder,
  ECS *const ecs
) {
  ecs__mark_for_destruction(launcher, ecs);
}

void (*on_entity_collide_with_radial_launcher_ptr)(
  EntityId launcher,
  EntityId entity,
  ECS *const ecs
);

// TODO: cheeky
double (*get_seconds_since_creation_ptr)();

void launchers__init_scene_callbacks(
  void (*on_entity_collide_with_radial_launcher)(
    EntityId launcher,
    EntityId entity,
    ECS *const ecs
  ),
  double (*const get_seconds_since_creation)()
) {
  on_entity_collide_with_radial_launcher_ptr =
    on_entity_collide_with_radial_launcher;
  get_seconds_since_creation_ptr = get_seconds_since_creation;
}

EntityId create_radial_launcher(
  Vec3 position,
  ECS *const ecs
) {

  EntityId launcher = ecs__create_entity(ecs);

  ecs__add_transform(
    launcher,
    (Transform){
      .position = position,
      .rotation = (Quaternion){ (Vec3){0}, 0 },
      .scale = 0.7f
    },
    ecs
  );
  ecs__add_look_at_center(launcher, ecs);
  ecs__add_alpha_effect(launcher, ecs);
  ecs__add_draw(
    launcher,
    (Draw){
      .textures = ARROW_TEXTURE,
      .draw = ecs__draw_mesh,
      .shader = &FLAT_TEXTURE_SHADER,
      .mesh = &QUAD
    },
    ecs
  );
  ecs__add_radius_collider(launcher, 0.15f, ecs);
  ecs__add_pickupable(
    launcher,
    on_entity_collide_with_radial_launcher_ptr,
    ecs
  );
  static Vec2 UV_SCROLL_SPEED = (Vec2){ 0, -6 };
  ecs__add_uv_scroll(
    launcher,
    (ScrollUV){
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
  ECS *const ecs
) {

  static float radians;
  radians += M_PI * 0.07f;

  Vec3 start = space__ccw_angle_rotate(
    WORLDSPACE.up,
    radians,
    (Vec3){ 0, 0, -START_RADIUS }
  );
  Vec3 end = space__ccw_angle_rotate(
    WORLDSPACE.up,
    radians,
    (Vec3){ 0, 0, -END_RADIUS }
  );

  EntityId launcher = create_radial_launcher(start, ecs);

  ecs__add_vec3lerp(
    launcher,
    (Vec3Lerp){
      .start = start,
      .end = end,
      .age = remainder,
      .duration = 1000.0, // TODO: base on distance spawned at
      .lerp = vec3__linear_lerp,
      .on_finish = destroy_launcher
    },
    ecs
  );
}

void launch_entity_radially(
  Vec3 start_position,
  Vec3 end_position,
  Seconds travel_time,
  void (*on_reach_destination)(
    EntityId entity,
    Seconds remainder,
    ECS *const ecs
  ),
  EntityId entity,
  ECS *const ecs
) {
  // TODO: replace with ecs__remove_controller when that's done
  // ecs__remove_player_controller(entity, ecs);
  ecs__remove_look_at_center(entity, ecs);
  ecs->entities[entity].draw.shader = &SOLID_COLOR_SHADER;
  ecs__remove_velocity(entity, ecs);
  ecs__add_vec3lerp(
    entity,
    (Vec3Lerp){
      .start = start_position,
      .end = end_position,
      .age = 0,
      .duration = travel_time,
      .lerp = vec3__linear_lerp,
      .on_finish = on_reach_destination
    },
    ecs
  );
  ecs__add_repeat(
    entity,
    (Repeat){
      .age = 0,
      .interval = 0.01f,
      .on_interval = create_afterimage
    },
    ecs
  );
}

void create_radial_launcher_spawner(
  ECS *const ecs
) {

  EntityId spawner = ecs__create_entity(ecs);

  ecs__add_repeat(
    spawner,
    (Repeat){
      .age = 0,
      .interval = 1.5f,
      .on_interval = spawn_radial_launcher
    },
    ecs
  );
}
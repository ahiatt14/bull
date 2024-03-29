#include <stdio.h>

#include "tail.h"

#include "ecs_types.h"
#include "ecs_systems.h"
#include "ecs_component_fns.h"
#include "ecs_drawing.h"

// TODO: we can make a Controller injectable
// that's agnostic about player/AI
#include "player.h"

#include "lighting.h"
#include "constants.h"
#include "bull_math.h"
#include "tail_helpers.h"

void ecs__control_player(
  GameTime time,
  Gamepad gamepad,
  ControllerActions const *const actions,
  Entity *const player,
  ECS *const ecs
) {

  // TODO: we should definitely create a Controller API
  // to inject and move the player-specific stuff out
  static const float SPEED = 5.0f;
  static const float STICK_DEADZONE = 0.2f;
  static const float TRIGGER_DEADZONE = 0.3f;

  if (lacks_components(c_PLAYER_CONTROLLER, player->config)) return;

  // TODO: not as clean as the FSM we had before, hmm
  // something to think about
  if (
    (has_component(c_REPEAT, player->config) == 0) &&
    gamepad.right_trigger >= TRIGGER_DEADZONE
  ) actions->on_start_autofire();

  if (
    has_component(c_REPEAT, player->config) &&
    gamepad.right_trigger < TRIGGER_DEADZONE
  ) actions->on_stop_autofire();

  Vec2 norm_direction =
    vec2__normalize(gamepad.left_stick_direction);
  float magnitude =
    vec2__magnitude(gamepad.left_stick_direction);

  if (magnitude < STICK_DEADZONE) {
    player->velocity = (Vec3){0};
    return;
  }

  player->velocity.x =
    SPEED * (magnitude + 1.0f) * magnitude *
    norm_direction.x;

  player->velocity.z =
    SPEED * (magnitude + 1.0f) * magnitude *
    norm_direction.y;

  pose_player_mech(time, player, ecs);
}

void ecs__gravity(
  GameTime time,
  ECS *const ecs
) {

  Vec3 acceleration = {0, -9.8 * time.delta, 0};

  for (EntityId id = 0; id < ecs->count; id++) {

    if (lacks_components(
      c_GRAVITY | c_VELOCITY,
      ecs->entities[id].config
    )) continue;

    ecs->entities[id].velocity = vec3_plus_vec3(
      ecs->entities[id].velocity,
      acceleration
    ); 
  }
}

void ecs__move(
  GameTime time,
  ECS *const ecs
) {

  for (EntityId id = 0; id < ecs->count; id++) {

    if (lacks_components(
      c_TRANSFORM | c_VELOCITY,
      ecs->entities[id].config
    )) continue;

    ecs->entities[id].transform.position = vec3_plus_vec3(
      ecs->entities[id].transform.position,
      scalar_x_vec3(time.delta, ecs->entities[id].velocity)
    );
  }
}

void ecs__timeout(
  GameTime time,
  ECS *const ecs
) {
  
  for (EntityId id = 0; id < ecs->count; id++) {

    if (lacks_components(
      c_TIMEOUT,
      ecs->entities[id].config
    )) continue;

    ecs->entities[id].timeout.age += time.delta;
    if (
      ecs->entities[id].timeout.age >=
      ecs->entities[id].timeout.limit
    ) ecs->entities[id].timeout.on_timeout(
      id,
      ecs->entities[id].timeout.age - ecs->entities[id].timeout.limit,
      ecs
    );
  }
}

void ecs__repeat(
  GameTime time,
  ECS *const ecs
) {

  Seconds age, interval, remainder;

  for (EntityId id = 0; id < ecs->count; id++) {

    if (lacks_components(
      c_REPEAT,
      ecs->entities[id].config
    )) continue;

    ecs->entities[id].repeat.age += time.delta;
    age = ecs->entities[id].repeat.age;
    interval = ecs->entities[id].repeat.interval;

    if (age >= interval) {
      remainder = age - interval;
      ecs->entities[id].repeat.on_interval(id, remainder, ecs);
      ecs->entities[id].repeat.age = remainder;
    }
  }
}

void ecs__scroll_uvs(
  GameTime time,
  ECS *const ecs
) {

  for (EntityId id = 0; id < ecs->count; id++) {

    if (lacks_components(c_UV_SCROLL, ecs->entities[id].config)) continue;

    ecs->entities[id].scroll_uv.total.x +=
      ecs->entities[id].scroll_uv.speed.x * time.delta; 
    ecs->entities[id].scroll_uv.total.y +=
      ecs->entities[id].scroll_uv.speed.y * time.delta;
  }
}

void ecs__lerp_vec3(
  GameTime time,
  ECS *const ecs
) {

  double ratio;

  for (EntityId id = 0; id < ecs->count; id++) {

    if (lacks_components(
      c_VEC3_LERP,
      ecs->entities[id].config
    )) continue;

    ecs->entities[id].vec3lerp.age += time.delta;

    ratio =
      ecs->entities[id].vec3lerp.age /
      ecs->entities[id].vec3lerp.duration;

    ecs->entities[id].transform.position =
      ecs->entities[id].vec3lerp.lerp(
        ecs->entities[id].vec3lerp.start,
        ecs->entities[id].vec3lerp.end,
        d_max(ratio, 1.0f)
      );

    if (ratio >= 1.0f) ecs->entities[id].vec3lerp.on_finish(
      id,
      ecs->entities[id].vec3lerp.age -
      ecs->entities[id].vec3lerp.duration,
      ecs
    );
  }
}

void ecs__lerp_revolve(
  GameTime time,
  ECS *const ecs
) {

  float ratio, rads;

  for (EntityId id = 0; id < ecs->count; id++) {

    if (lacks_components(
      c_REVOLVE_LERP,
      ecs->entities[id].config
    )) continue;

    ecs->entities[id].revolve_lerp.age += time.delta;

    ratio =
      ecs->entities[id].revolve_lerp.age /
      ecs->entities[id].revolve_lerp.duration;

    rads =
      d_max(ratio, 1.0f) *
      ecs->entities[id].revolve_lerp.target_rads;

    ecs->entities[id].transform.position =
      space__ccw_angle_rotate(
        WORLDSPACE.up,
        rads,
        ecs->entities[id].revolve_lerp.start
      );

    if (ratio >= 1.0f) ecs->entities[id].revolve_lerp.on_finish(
      id,
      ecs->entities[id].revolve_lerp.age -
      ecs->entities[id].revolve_lerp.duration,
      ecs
    );
  }
}

void ecs__lerp_rotation(
  GameTime time,
  ECS *const ecs
) {

  Entity *entity;

  for (EntityId id = 0; id < ecs->count; id++) {

    entity = &ecs->entities[id];

    if (lacks_components(c_ROTATION_LERP, entity->config)) continue;

    entity->rotation_lerp.age += time.delta;

    float ratio =
      entity->rotation_lerp.age / 
      entity->rotation_lerp.duration;

    entity->transform.rotation =
      quaternion__linear_slerp(
        entity->rotation_lerp.start,
        entity->rotation_lerp.target,
        ratio
      );

    if (ratio < 1.0f) continue;

    entity->rotation_lerp.on_finish(
      id,
      entity->rotation_lerp.age - entity->rotation_lerp.duration,
      ecs
    );
  }
}

void ecs__look_at_center(
  GameTime time,
  ECS *const ecs
) {

  Vec3 forward;

  for (EntityId id = 0; id < ecs->count; id++) {

    if (lacks_components(
      c_LOOK_AT_CENTER,
      ecs->entities[id].config
    )) continue;

    forward = vec3__normalize(vec3_minus_vec3(
      ORIGIN,
      ecs->entities[id].transform.position
    ));

    ecs->entities[id].transform.rotation =
      quaternion__create(
        WORLDSPACE.up,
        rads_ccw_from_forward_around_up(forward)
      );
  }
}

// TODO: learn how to do efficient collision detection in ECS!
void ecs__check_projectile_radius_collisions(
  GameTime time,
  ECS *const ecs
) {

  float distance_between_positions, damagable_radius, projectile_radius;

  for (EntityId id = 0; id < ecs->count; id++) {

    if (lacks_components(
      c_DAMAGABLE | c_RADIUS_COLLIDER,
      ecs->entities[id].config
    )) continue;

    for (EntityId projectile = 0; projectile < ecs->count; projectile++) {

      if (lacks_components(
        c_RADIUS_COLLIDER | c_DAMAGER,
        ecs->entities[projectile].config
      )) continue;

      distance_between_positions = vec3__distance(
        ecs->entities[id].transform.position,
        ecs->entities[projectile].transform.position
      );

      damagable_radius = ecs->entities[id].radius;
      projectile_radius = ecs->entities[projectile].radius;

      if (
        (distance_between_positions - damagable_radius - projectile_radius) >
        (damagable_radius + projectile_radius)
      ) continue;

      ecs->entities[id].on_hit_by_damager(
        id,
        projectile,
        ecs
      );
    }
  }
}

void ecs__check_pickup_radius_collisions(
  GameTime time,
  ECS *const ecs
) {

  // TODO: not very ECS-y of us, think about pickup interaction
  if (lacks_components(
    c_PLAYER_CONTROLLER,
    ecs->entities[PLAYER_ID].config
  )) return;

  float distance_between_positions, pickup_radius;

  float player_radius = ecs->entities[PLAYER_ID].radius;
  Vec3 player_position = ecs->entities[PLAYER_ID].transform.position;

  for (EntityId pickup = 0; pickup < ecs->count; pickup++) {

    if (lacks_components(
      c_PICKUPABLE | c_RADIUS_COLLIDER,
      ecs->entities[pickup].config
    )) continue;

    distance_between_positions = vec3__distance(
      ecs->entities[pickup].transform.position,
      player_position
    );

    pickup_radius = ecs->entities[pickup].radius;

    if (
      (distance_between_positions - pickup_radius - player_radius) >
      (pickup_radius + player_radius)
    ) continue;

    ecs->entities[pickup].on_picked_up(pickup, PLAYER_ID, ecs);
  }
}

void ecs__draw(
  GameTime time,
  Camera const *const camera,
  Lighting *const lighting,
  GPU const *const gpu,
  ECS *const ecs
) {

  static EntityId alpha_entities[MAX_ENTITIES];
  static uint_fast16_t alpha_entity_count;
  alpha_entity_count = 0;

  lighting->point_count = 0;
  for (EntityId id = 0; id < ecs->count; id++)
    if (has_component(c_POINT_LIGHT, ecs->entities[id].config))
      lighting->point_sources[lighting->point_count++] = id;

  gpu->cull_back_faces();

  for (EntityId id = 0; id < ecs->count; id++) {

    if (lacks_components(c_DRAW, ecs->entities[id].config))
      continue;

    if (has_component(c_ALPHA_EFFECT, ecs->entities[id].config)) {
      alpha_entities[alpha_entity_count++] = id;
      continue;
    }
    
    ecs__prepare_entity_draw(
      time,
      camera,
      gpu,
      lighting,
      id,
      ecs
    );
  }

  sort_alpha_entities(
    camera->position,
    alpha_entities,
    alpha_entity_count,
    ecs
  );

  for (uint_fast16_t i = 0; i < alpha_entity_count; i++)
    ecs__prepare_entity_draw(
      time,
      camera,
      gpu,
      lighting,
      alpha_entities[i],
      ecs
    );
}
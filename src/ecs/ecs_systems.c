#include "tail.h"

#include "ecs_types.h"
#include "ecs_systems.h"
#include "ecs_component_fns.h"

// TODO: we can make a Controller injectable
// that's agnostic about player/AI
#include "player.h"

#include "constants.h"
#include "bull_math.h"
#include "tail_helpers.h"

void ecs__control_player(
  struct GameTime time,
  struct Gamepad gamepad,
  struct ControllerActions const *const actions,
  struct Entity *const player
) {

  // TODO: we should definitely create a Controller API
  // to inject and move the player-specific stuff out
  static const float SPEED = 5.0f;
  static const float STICK_DEADZONE = 0.2f;
  static const float TRIGGER_DEADZONE = 0.3f;

  if (lacks_configuration(c_PLAYER_CONTROLLER, player->config)) return;

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

  struct Vec2 norm_direction =
    vec2__normalize(gamepad.left_stick_direction);
  float magnitude =
    vec2__magnitude(gamepad.left_stick_direction);

  if (magnitude < STICK_DEADZONE) {
    player->velocity = (struct Vec3){0};
    return;
  }

  player->velocity.x =
    SPEED * (magnitude + 1.0f) * magnitude *
    norm_direction.x;

  player->velocity.z =
    SPEED * (magnitude + 1.0f) * magnitude *
    norm_direction.y;
}

void ecs__gravity(
  struct GameTime time,
  struct ECS *const ecs
) {

  struct Vec3 acceleration = {0, -3 * time.delta, 0};

  for (EntityId id = 0; id < ecs->count; id++) {

    if (lacks_configuration(
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
  struct GameTime time,
  struct ECS *const ecs
) {

  for (EntityId id = 0; id < ecs->count; id++) {

    if (lacks_configuration(
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
  struct GameTime time,
  struct ECS *const ecs
) {
  
  for (EntityId id = 0; id < ecs->count; id++) {

    if (lacks_configuration(
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
  struct GameTime time,
  struct ECS *const ecs
) {

  Seconds age, interval, remainder;

  for (EntityId id = 0; id < ecs->count; id++) {

    if (lacks_configuration(
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
  struct GameTime time,
  struct ECS *const ecs
) {

  for (EntityId id = 0; id < ecs->count; id++) {

    if (lacks_configuration(
      c_DRAW_MESH | c_UV_SCROLL,
      ecs->entities[id].config
    )) continue;

    ecs->entities[id].draw.uv_scroll_total.x +=
      ecs->entities[id].draw.uv_scroll_speed.x * time.delta; 
    ecs->entities[id].draw.uv_scroll_total.y +=
      ecs->entities[id].draw.uv_scroll_speed.y * time.delta;
  }
}

void ecs__lerp_vec3(
  struct GameTime time,
  struct ECS *const ecs
) {

  double ratio;

  for (EntityId id = 0; id < ecs->count; id++) {

    if (lacks_configuration(
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
        dmax(ratio, 1.0f)
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
  struct GameTime time,
  struct ECS *const ecs
) {

  float ratio, rads;

  for (EntityId id = 0; id < ecs->count; id++) {

    if (lacks_configuration(
      c_REVOLVE_LERP,
      ecs->entities[id].config
    )) continue;

    ecs->entities[id].revolve_lerp.age += time.delta;

    ratio =
      ecs->entities[id].revolve_lerp.age /
      ecs->entities[id].revolve_lerp.duration;

    rads =
      dmax(ratio, 1.0f) *
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

void ecs__look_at_center(
  struct GameTime time,
  struct ECS *const ecs
) {

  struct Vec3 forward;

  for (EntityId id = 0; id < ecs->count; id++) {

    if (lacks_configuration(
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

void ecs__draw(
  struct GameTime time,
  struct Camera const *const cam,
  struct GPU const *const gpu,
  struct ECS *const ecs
) {

  // TODO: boy this is getting messy

  for (EntityId id = 0; id < ecs->count; id++) {

    if (
      lacks_configuration(c_DRAW_BILLBOARD, ecs->entities[id].config) &&
      lacks_configuration(c_DRAW_MESH, ecs->entities[id].config)
    ) continue;

    struct Shader *shad = ecs->entities[id].draw.shader;

    gpu->select_shader(shad);

    if (ecs->entities[id].draw.texture != NULL)
      gpu->select_texture(ecs->entities[id].draw.texture);

    gpu->set_shader_m4x4(
      shad,
      "view",
      &cam->lookat
    );
    gpu->set_shader_m4x4(
      shad,
      "projection",
      &cam->projection
    );
    gpu->set_shader_float(
      shad,
      "total_elapsed_seconds",
      time.sec_since_game_launch
    );
    if (has_component(c_UV_SCROLL, ecs->entities[id].config)) {
      gpu->set_shader_vec2(
        shad,
        "total_uv_scroll",
        ecs->entities[id].draw.uv_scroll_total
      );
    }
    if (has_component(c_TIMEOUT, ecs->entities[id].config)) {
      gpu->set_shader_float(
        shad,
        "seconds_since_activation",
        ecs->entities[id].timeout.age
      );
      gpu->set_shader_float(
        shad,
        "limit_in_seconds",
        ecs->entities[id].timeout.limit
      );
    }

    struct M4x4 local_to_world;
    struct M3x3 normals_local_to_world;

    if (has_component(
      c_DRAW_BILLBOARD,
      ecs->entities[id].config
    )) {

      m4x4__translation(
        ecs->entities[id].transform.position,
        &local_to_world
      );
      gpu->set_shader_float(
        shad,
        "scale",
        ecs->entities[id].transform.scale
      );
      gpu->set_shader_m4x4(
        shad,
        "model",
        &local_to_world
      );

      gpu->draw_points(&POINT); 

    } else {

      space__create_model(
        &WORLDSPACE,
        &ecs->entities[id].transform,
        &local_to_world
      );
      space__create_normals_model(
        &local_to_world,
        &normals_local_to_world
      );

      gpu->set_shader_m3x3(
        shad,
        "normals_model",
        &normals_local_to_world
      );
      gpu->set_shader_m4x4(
        shad,
        "model",
        &local_to_world
      );

      gpu->draw_mesh(ecs->entities[id].draw.mesh);
    }
  }
}
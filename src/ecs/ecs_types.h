#ifndef __BULL_ECS_TYPES__
#define __BULL_ECS_TYPES__

#include <stdint.h>

#include "tail.h"
#include "constants.h"
#include "tail_helpers.h"

#define MAX_ENTITIES 500

// TODO: reorder these at some point
#define c_TRANSFORM 1 << 0
#define c_VELOCITY 1 << 1
#define c_TIMEOUT 1 << 2
#define c_DRAW 1 << 3
#define c_ROTATION_LERP 1 << 4
#define c_UV_SCROLL 1 << 5
#define c_VEC3_LERP 1 << 6
#define c_REVOLVE_LERP 1 << 7
#define c_LOOK_AT_CENTER 1 << 8
#define c_PLAYER_CONTROLLER 1 << 9
#define c_REPEAT 1 << 10
#define c_WEAPONS 1 << 11
#define c_GRAVITY 1 << 12
#define c_DRAW_BACK_FACES 1 << 13
#define c_RADIUS_COLLIDER 1 << 14
#define c_DAMAGER 1 << 15
#define c_DAMAGABLE 1 << 16
#define c_PICKUPABLE 1 << 17
#define c_ALPHA_EFFECT 1 << 18

typedef uint_fast16_t EntityId;
typedef uint_fast32_t ComponentConfig;

struct ECS;
struct Entity;

struct Weapons {
  void (*primary)(
    EntityId vehicle,
    Seconds remainder,
    struct ECS *const ecs
  );
  Seconds primary_autofire_interval;
  // void (*secondary)(
  //   EntityId vehicle,
  //   Seconds remainder,
  //   struct ECS *const ecs
  // );
  // Seconds secondary_autofire_interval;
};

struct RadiusCollider {
  float radius;
  void (*on_collide)(
    EntityId id,
    EntityId other,
    struct ECS *const ecs
  );
};

struct Timeout {
  Seconds age;
  Seconds limit;
  void (*on_timeout)(
    EntityId id,
    Seconds remainder,
    struct ECS *const ecs
  );
};

struct Repeat {
  Seconds age;
  Seconds interval;
  void (*on_interval)(
    EntityId id,
    Seconds remainder,
    struct ECS *const ecs
  );
};

struct Draw {
  struct DrawableMesh *mesh;
  uint_fast16_t textures;
  struct Shader *shader;
  void (*draw)(
    struct GameTime time,
    struct Camera const *const cam,
    struct GPU const *const gpu,
    struct Entity const *const entity
  );
};

struct ScrollUV {
  struct Vec2 speed;
  struct Vec2 total;
};

struct Vec3Lerp {
  struct Vec3 start;
  struct Vec3 end;
  Seconds age;
  Seconds duration;
  struct Vec3 (*lerp)(
    struct Vec3 start,
    struct Vec3 end,
    double ratio
  );
  void (*on_finish)(
    EntityId id,
    Seconds remainder,
    struct ECS *const ecs
  );
};

struct RevolveLerp {
  struct Vec3 start;
  float target_rads;
  Seconds age;
  Seconds duration;
  void (*on_finish)(
    EntityId id,
    Seconds remainder,
    struct ECS *const ecs
  );
};

struct RotationLerp {
  struct Quaternion start;
  struct Quaternion target;
  Seconds age;
  Seconds duration;
};

// TODO: AoS vs SoA blah blah blah
// TODO: do hot/cold components?
struct Entity {
  struct Transform transform;
  struct Timeout timeout;
  struct Repeat repeat;
  struct RevolveLerp revolve_lerp;
  struct Vec3Lerp vec3lerp;
  struct RotationLerp rotation_lerp;
  struct Vec3 velocity;
  struct ScrollUV scroll_uv;
  struct Draw draw;
  struct Weapons weapons;
  float radius;
  void (*on_hit_by_damager)(
    EntityId collidee,
    EntityId collider,
    struct ECS *const ecs
  );
  ComponentConfig config;
};

struct ECS {
  struct Entity entities[MAX_ENTITIES];
  uint_fast16_t count;
  uint_fast16_t entities_to_destroy[MAX_ENTITIES];
  uint_fast16_t entities_to_destroy_count;
};

#endif
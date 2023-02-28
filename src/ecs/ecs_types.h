#ifndef __BULL_ECS_TYPES__
#define __BULL_ECS_TYPES__

#include <stdint.h>

#include "tail.h"
#include "constants.h"
#include "tail_helpers.h"

#define MAX_ENTITIES 1000

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

typedef struct BULLECS ECS;
typedef struct BULLENTITY Entity;

typedef struct BULLWEAPONS {
  void (*primary)(
    EntityId vehicle,
    Seconds remainder,
    ECS *const ecs
  );
  Seconds primary_autofire_interval;
  // void (*secondary)(
  //   EntityId vehicle,
  //   Seconds remainder,
  //   ECS *const ecs
  // );
  // Seconds secondary_autofire_interval;
} Weapons;

typedef struct BULLTIMEOUT {
  Seconds age;
  Seconds limit;
  void (*on_timeout)(
    EntityId id,
    Seconds remainder,
    ECS *const ecs
  );
} Timeout;

typedef struct BULLREPEAT {
  Seconds age;
  Seconds interval;
  void (*on_interval)(
    EntityId id,
    Seconds remainder,
    ECS *const ecs
  );
} Repeat;

typedef struct BULLDRAW {
  DrawableMesh *mesh;
  uint_fast16_t textures;
  Shader *shader;
  void (*draw)(
    GameTime time,
    Camera const *const cam,
    GPU const *const gpu,
    Entity const *const entity
  );
} Draw;

typedef struct BULLSCROLLUV {
  Vec2 speed;
  Vec2 total;
} ScrollUV;

typedef struct BULLVEC3LERP {
  Vec3 start;
  Vec3 end;
  Seconds age;
  Seconds duration;
  Vec3 (*lerp)(
    Vec3 start,
    Vec3 end,
    double ratio
  );
  void (*on_finish)(
    EntityId id,
    Seconds remainder,
    ECS *const ecs
  );
} Vec3Lerp;

typedef struct BULLREVOLVELERP {
  Vec3 start;
  float target_rads;
  Seconds age;
  Seconds duration;
  void (*on_finish)(
    EntityId id,
    Seconds remainder,
    ECS *const ecs
  );
} RevolveLerp;

typedef struct BULLROTATIONLERP {
  Quaternion start;
  Quaternion target;
  Seconds age;
  Seconds duration;
} RotationLerp;

// TODO: AoS vs SoA blah blah blah
// TODO: do hot/cold components?
struct BULLENTITY {
  Transform transform;
  Timeout timeout;
  Repeat repeat;
  RevolveLerp revolve_lerp;
  Vec3Lerp vec3lerp;
  RotationLerp rotation_lerp;
  Vec3 velocity;
  ScrollUV scroll_uv;
  Draw draw;
  Weapons weapons;
  float radius;
  void (*on_hit_by_damager)(
    EntityId collidee,
    EntityId collider,
    ECS *const ecs
  );
  void (*on_picked_up)(
    EntityId pickupable,
    EntityId pickuper,
    ECS *const ecs
  );
  ComponentConfig config;
};

struct BULLECS {
  Entity entities[MAX_ENTITIES];
  uint_fast16_t count;
  uint_fast16_t entities_to_destroy[MAX_ENTITIES];
  uint_fast16_t entities_to_destroy_count;
};

#endif
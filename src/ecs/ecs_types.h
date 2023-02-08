#ifndef __BULL_ECS_TYPES__
#define __BULL_ECS_TYPES__

#include <stdint.h>

#include "tail.h"
#include "constants.h"

#define MAX_ENTITIES 500

#define c_TRANSFORM 1 << 0
#define c_VELOCITY 1 << 1
#define c_TIMEOUT 1 << 2
#define c_DRAW_BILLBOARD 1 << 3
#define c_DRAW_MESH 1 << 4
#define c_UV_SCROLL 1 << 5
#define c_VEC3_LERP 1 << 6
#define c_REVOLVE_LERP 1 << 7
#define c_LOOK_AT_CENTER 1 << 8
#define c_PLAYER_CONTROLLER 1 << 9
#define c_REPEAT 1 << 10
#define c_WEAPONS 1 << 11

typedef uint_fast16_t EntityId;
typedef uint_fast16_t ComponentConfig;

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
};

struct Timeout {
  Seconds age;
  Seconds limit;
  void (*on_timeout)(
    EntityId id,
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

// TODO: should we make a separate structure to hold these
// and turn it into a cold member i.e. just a pointer
// to the whole draw component
struct Draw {
  struct DrawableMesh *mesh;
  struct Texture *texture;
  struct Shader *shader;
  struct Vec2 uv_scroll_speed;
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

// TODO: do hot/cold components at some point
struct Entity {
  struct Transform transform;
  struct Timeout timeout;
  struct Repeat repeat;
  struct RevolveLerp revolve_lerp;
  struct Vec3Lerp vec3lerp;
  struct Vec3 velocity;
  struct Draw draw;
  struct Weapons weapons;
  ComponentConfig config;
};

struct ECS {
  struct Entity entities[MAX_ENTITIES];
  uint_fast16_t count;
};

#endif
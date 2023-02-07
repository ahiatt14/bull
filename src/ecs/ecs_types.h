#ifndef __BULL_ECS_TYPES__
#define __BULL_ECS_TYPES__

#include <stdint.h>

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

typedef uint_fast16_t EntityId;
typedef uint_fast16_t ComponentConfig;

struct ECS;
struct Entity;

struct Timeout {
  double seconds_since_activation;
  double limit_in_seconds;
  void (*on_timeout)(
    EntityId id,
    struct ECS *const ecs
  );
};

struct Repeat {
  double seconds_since_interval;
  double interval_in_seconds;
  void (*on_interval)(
    EntityId id,
    double remainder_in_seconds,
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
  double seconds_since_activation;
  double duration_in_seconds;
  struct Vec3 (*lerp)(
    struct Vec3 start,
    struct Vec3 end,
    double ratio
  );
  void (*on_finish)(
    EntityId id,
    double remainder_in_seconds,
    struct ECS *const ecs
  );
};

struct RevolveLerp {
  struct Vec3 start_position;
  float target_rads;
  double seconds_since_activation;
  double duration_in_seconds;
  void (*on_finish)(
    EntityId id,
    double remainder_in_seconds,
    struct ECS *const ecs
  );
};

// TODO: do hot/cold components at some point
struct Entity {
  struct Transform transform;
  struct Vec3Lerp vec3lerp;
  struct RevolveLerp revolve_lerp;
  struct Timeout timeout;
  struct Repeat repeat;
  struct Vec3 velocity;
  struct Draw draw;
  ComponentConfig config;
};

struct ECS {
  struct Entity entities[MAX_ENTITIES];
  uint_fast16_t count;
};

#endif
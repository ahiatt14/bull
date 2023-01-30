#ifndef __BULL_ECS_TYPES__
#define __BULL_ECS_TYPES__

#include <stdint.h>

#define MAX_ENTITIES 500

#define c_TRANSFORM 1
#define c_VELOCITY 2
#define c_TIMEOUT 4
#define c_DRAW_BILLBOARD 8
#define c_DRAW_MESH 16
#define c_UV_SCROLL 32
#define c_VEC3_LERP 64

typedef uint_fast16_t EntityId;
typedef uint_fast16_t ComponentConfig;

struct Timeout {
  double seconds_since_activation;
  double limit_in_seconds;
  void (*on_timeout)(EntityId id);
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
    double remainder_in_seconds
  );
};

struct Entity {
  ComponentConfig config;
  struct Transform transform;
  struct Vec3Lerp vec3lerp;
  struct Vec3 velocity;
  struct Timeout timeout;
  struct Draw draw;
};

struct ECS {
  struct Entity entities[MAX_ENTITIES];
  uint_fast16_t count;
};

#endif
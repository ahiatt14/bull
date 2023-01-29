#ifndef __BULL_ECS_DEFINITIONS__
#define __BULL_ECS_DEFINITIONS__

#include <stdint.h>

#include "tail.h"

#define MAX_ENTITIES 500

#define c_TRANSFORM 1
#define c_VELOCITY 2
#define c_TIMEOUT 4
#define c_DRAW_BILLBOARD 8
#define c_DRAW_MESH 16
#define c_UV_SCROLL 32

typedef uint_fast16_t EntityId;
typedef uint_fast16_t ComponentConfig;

struct Timeout {
  double seconds_since_activation;
  double limit_in_seconds;
  void (*on_timeout)(EntityId id);
};

struct Draw {
  struct DrawableMesh *mesh;
  struct Texture *texture;
  struct Shader *shader;
  struct Vec2 uv_scroll_speed;
};

struct Entity {
  ComponentConfig config;
  struct Transform transform;
  struct Vec3 velocity;
  struct Timeout timeout;
  struct Draw draw;
};

struct ECS {
  struct Entity entities[MAX_ENTITIES];
  uint_fast16_t count;
};

#endif
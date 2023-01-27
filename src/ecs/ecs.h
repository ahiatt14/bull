#ifndef __BULL_ECS__
#define __BULL_ECS__

#include "tail.h"

#include "tail_helpers.h"

#define MAX_ENTITIES 256

#define c_TRANSFORM 1
#define c_VELOCITY 2
#define c_TIMEOUT 4
#define c_DRAW_BILLBOARD 8
#define c_DRAW_MESH 16
// #define c_HAS_PARENT 32
// #define c_HAS_CHILD 64
#define c_UV_SCROLL 128
// #define c_HITPOINTS 256
// #define c_AREA 512

typedef uint_fast16_t entity_id;
typedef uint_fast16_t component_config;

struct timeout {
  double seconds_since_activation;
  double limit_in_seconds;
  void (*on_timeout)(entity_id id);
};

// struct hierarchy {
//   entity_id parent_id;
//   entity_id child_id;
// };

struct draw {
  struct drawable_mesh *mesh;
  struct texture *texture;
  struct shader *shader;
  struct vec2 uv_scroll_speed;
};

struct entity {
  component_config config;
  struct transform transform;
  struct vec3 velocity;
  struct timeout timeout;
  // struct hierarchy hierarchy;
  struct draw draw;
};

struct entities {
  struct entity _entities[MAX_ENTITIES];
  uint_fast16_t _active_count;
};

uint_fast16_t get_active_entity_count();

struct entity* ecs__get_entity(
  entity_id id
);

entity_id ecs__create_entity();

void ecs__destroy_entity(
  entity_id id
);

void ecs__add_transform(
  entity_id id,
  struct transform t
);

void ecs__add_draw_mesh(
  entity_id id,
  struct draw drw
);

void ecs__add_velocity(
  entity_id id,
  struct vec3 velocity
);

void ecs__remove_velocity(
  entity_id id
);

void ecs__add_timeout(
  entity_id id,
  struct timeout t
);

void ecs__remove_timeout(
  entity_id id
);

void ecs__move(
  struct gametime time
);

void ecs__timeout(
  struct gametime time
);

void ecs__scroll_uvs(
  struct gametime time
);

void ecs__draw(
  struct gametime time,
  struct camera const *const cam,
  struct gpu_api const *const gpu
);

#endif
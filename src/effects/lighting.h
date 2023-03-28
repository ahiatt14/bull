#ifndef __BULL_LIGHTING__
#define __BULL_LIGHTING__

#include <stdint.h>

#include "tail.h"

// #include "ecs.h"

#define MAX_POINT_LIGHTS 20

typedef struct BULLAMBIENTLIGHT AmbientLight;
typedef struct BULLSKYLIGHT SkyLight;
typedef struct BULLLIGHTING Lighting;

struct BULLAMBIENTLIGHT {
  Vec3 color;
  float strength;
};

struct BULLSKYLIGHT {
  Vec3 direction;
  Vec3 color;
  float strength;
};

struct BULLLIGHTING {
  uint_fast8_t point_count;
  // EntityId point_sources[MAX_POINT_LIGHTS];
  uint_fast16_t point_sources[MAX_POINT_LIGHTS];
  SkyLight sky;
  AmbientLight ambient;
};

#endif
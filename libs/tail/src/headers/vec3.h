#ifndef __TAIL_VEC3__
#define __TAIL_VEC3__

#include "precision.h"

typedef struct VEC3 {
  float x, y, z;
} vec3;

void vec3_create(float x, float y, float z, vec3 *t);
void vec3_plus_vec3(const vec3 *t0, const vec3 *t1, vec3 *sum);
void vec3_minus_vec3(const vec3 *t0, const vec3 *t1, vec3 *diff);
float vec3_magnitude(const vec3 *t);
void vec3_normalize(const vec3 *t, vec3 *normalized);
void vec3_mean(const vec3 *vec3s, int count, vec3 *avg);
float vec3_dot(const vec3 *t0, const vec3 *t1);
void vec3_cross(const vec3 *t0, const vec3 *t1, vec3 *cross);
int vec3_equals_vec3(const vec3 *t0, const vec3 *t1, const float_tolerance *ft);

#endif
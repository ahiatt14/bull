#ifndef __TAIL_M4X4__
#define __TAIL_M4X4__

#include "vec3.h"

struct m4x4 {
  float data[16];
};

void m4x4_create(
  float i0, float i1, float i2, float i3,
  float i4, float i5, float i6, float i7,
  float i8, float i9, float i10, float i11,
  float i12, float i13, float i14, float i15,
  struct m4x4 *m
);
void m4x4_x_m4x4(
  const struct m4x4 *m0,
  const struct m4x4 *m1,
  struct m4x4 *dest
);
void m4x4_transpose(struct m4x4 *m);
void m4x4_identity(struct m4x4 *m);
void m4x4_x_point(const struct m4x4 *m, const struct vec3 *t, struct vec3 *dest);
void m4x4_translation(const struct vec3 *t, struct m4x4 *m);
void m4x4_inverted_translation(const struct vec3 *t, struct m4x4 *m);
void m4x4_scaling(float s, struct m4x4 *m);
void m4x4_rotation(float rads, const struct vec3 *t, struct m4x4 *m);
void m4x4_view(
  const struct vec3 *right,
  const struct vec3 *up,
  const struct vec3 *forward,
  struct m4x4 *m
);

#endif
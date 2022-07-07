#ifndef __TAIL_M4X4__
#define __TAIL_M4X4__

#include "vector.h"
#include "m3x3.h"

struct m4x4 {
  float data[16];
};

void m4x4__create(
  float i0, float i1, float i2, float i3,
  float i4, float i5, float i6, float i7,
  float i8, float i9, float i10, float i11,
  float i12, float i13, float i14, float i15,
  struct m4x4 *const m
);
void m4x4_x_m4x4(
  struct m4x4 const *const m0,
  struct m4x4 const *const m1,
  struct m4x4 *const dest
);
void m4x4__sub3x3_from00(struct m4x4 const *const src, struct m3x3 *const dest);
void m4x4__transpose(struct m4x4 *const m);
void m4x4__identity(struct m4x4 *const m);
void m4x4_x_point(
  struct m4x4 const *const m,
  struct vec3 const *const t,
  struct vec3 *const dest
);
void m4x4__translation(struct vec3 const *const t, struct m4x4 *const m);
void m4x4__inverted_translation(
  struct vec3 const *const t,
  struct m4x4 *const m
);
void m4x4__scaling(float s, struct m4x4 *const m);
void m4x4__rotation(
  float rads,
  struct vec3 const *const axis,
  struct m4x4 *const m
);
void m4x4__view(
  struct vec3 const *const right,
  struct vec3 const *const up,
  struct vec3 const *const forward,
  struct m4x4 *const m
);

#endif
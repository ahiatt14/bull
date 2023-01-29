#ifndef __TAIL_M4X4__
#define __TAIL_M4X4__

#include "vector.h"
#include "m3x3.h"

struct M4x4 {
  float data[16];
};

void m4x4__create(
  float i0, float i1, float i2, float i3,
  float i4, float i5, float i6, float i7,
  float i8, float i9, float i10, float i11,
  float i12, float i13, float i14, float i15,
  struct M4x4 *const m
);
void m4x4_x_m4x4(
  struct M4x4 const *const m0,
  struct M4x4 const *const m1,
  struct M4x4 *const dest
);
void m4x4__sub3x3_from00(
  struct M4x4 const *const src,
  struct M3x3 *const dest
);
void m4x4__transpose(struct M4x4 *const m);
void m4x4__identity(struct M4x4 *const m);
struct Vec3 m4x4_x_point(
  struct M4x4 const *const m,
  struct Vec3 t
);
void m4x4__translation(
  struct Vec3 t,
  struct M4x4 *const m
);
void m4x4__inverted_translation(
  struct Vec3 t,
  struct M4x4 *const m
);
void m4x4__scaling(
  float s,
  struct M4x4 *const m
);
void m4x4__rotation(
  float rads,
  struct Vec3 axis,
  struct M4x4 *const m
);
void m4x4__view(
  struct Vec3 right,
  struct Vec3 up,
  struct Vec3 forward,
  struct M4x4 *const m
);

#endif
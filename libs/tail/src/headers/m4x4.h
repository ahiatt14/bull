#ifndef __TAIL_M4X4__
#define __TAIL_M4X4__

#include "vector.h"
#include "m3x3.h"

typedef struct TAILM4X4 {
  float data[16];
} M4x4;

void m4x4__create(
  float i0, float i1, float i2, float i3,
  float i4, float i5, float i6, float i7,
  float i8, float i9, float i10, float i11,
  float i12, float i13, float i14, float i15,
  M4x4 *const m
);
void m4x4_x_m4x4(
  M4x4 const *const m0,
  M4x4 const *const m1,
  M4x4 *const dest
);
void m4x4__sub3x3_from00(
  M4x4 const *const src,
  M3x3 *const dest
);
void m4x4__transpose(
  M4x4 *const m
);
void m4x4__identity(
  M4x4 *const m
);
Vec3 m4x4_x_point(
  M4x4 const *const m,
  Vec3 t
);
void m4x4__translation(
  Vec3 t,
  M4x4 *const m
);
void m4x4__inverted_translation(
  Vec3 t,
  M4x4 *const m
);
void m4x4__scaling(
  float s,
  M4x4 *const m
);
void m4x4__rotation(
  float rads,
  Vec3 axis,
  M4x4 *const m
);
void m4x4__view(
  Vec3 right,
  Vec3 up,
  Vec3 forward,
  M4x4 *const m
);

#endif
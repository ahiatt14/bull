#ifndef __TAIL_M3X3__
#define __TAIL_M3X3__

#include "m3x3.h"

typedef struct TAILM3X3 {
  float data[9];
} M3x3;

void m3x3__create(
  float i0, float i3, float i6,
  float i1, float i4, float i7,
  float i2, float i5, float i8,
  M3x3 *const m
);
void m3x3__identity(
  M3x3 *const m
);
float m3x3__minor(
  int r,
  int c,
  M3x3 const *const m
);
void m3x3__minors(
  M3x3 const *const src,
  M3x3 *const dest
);
void m3x3__cofactors(
  M3x3 const *const src,
  M3x3 *const dest
);
void m3x3__transpose(
  M3x3 const *const src,
  M3x3 *const dest
);
float m3x3__determinant(
  M3x3 const *const m
);
void m3x3__inverse(
  M3x3 const *const src,
  M3x3 *const dest
);

#endif
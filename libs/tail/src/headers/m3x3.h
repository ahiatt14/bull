#ifndef __TAIL_M3X3__
#define __TAIL_M3X3__

#include "m3x3.h"

struct M3x3 {
  float data[9];
};

void m3x3__create(
  float i0, float i3, float i6,
  float i1, float i4, float i7,
  float i2, float i5, float i8,
  struct M3x3 *const m
);
void m3x3__identity(struct M3x3 *const m);
float m3x3__minor(int r, int c, struct M3x3 const *const m);
void m3x3__minors(struct M3x3 const *const src, struct M3x3 *const dest);
void m3x3__cofactors(struct M3x3 const *const src, struct M3x3 *const dest);
void m3x3__transpose(struct M3x3 const *const src, struct M3x3 *const dest);
float m3x3__determinant(struct M3x3 const *const m);
void m3x3__inverse(struct M3x3 const *const src, struct M3x3 *const dest);

#endif
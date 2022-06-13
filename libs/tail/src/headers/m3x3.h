#ifndef __TAIL_M3X3__
#define __TAIL_M3X3__

#include "m3x3.h"

struct m3x3 {
  float data[9];
};

void m3x3__create(
  float i0, float i3, float i6,
  float i1, float i4, float i7,
  float i2, float i5, float i8,
  struct m3x3 *m
);
void m3x3__identity(struct m3x3 *m);
float m3x3__minor(int r, int c, const struct m3x3 *m);
void m3x3__minors(const struct m3x3 *src, struct m3x3 *dest);
void m3x3__cofactors(const struct m3x3 *src, struct m3x3 *dest);
void m3x3__transpose(const struct m3x3 *src, struct m3x3 *dest);
float m3x3__determinant(const struct m3x3 *m);
void m3x3__inverse(const struct m3x3 *src, struct m3x3 *dest);

#endif
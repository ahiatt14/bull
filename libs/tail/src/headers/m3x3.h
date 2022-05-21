#ifndef __TAIL_M3X3__
#define __TAIL_M3X3__

#include "m3x3.h"

struct m3x3 {
  float data[9];
};

void m3x3_create(
  float i0, float i3, float i6,
  float i1, float i4, float i7,
  float i2, float i5, float i8,
  struct m3x3 *m
);
void m3x3_identity(struct m3x3 *m);
float m3x3_minor(int r, int c, const struct m3x3 *m);
void m3x3_minors(const struct m3x3 *src, struct m3x3 *dest);
void m3x3_cofactors(const struct m3x3 *src, struct m3x3 *dest);
void m3x3_transpose(const struct m3x3 *src, struct m3x3 *dest);
float m3x3_determinant(const struct m3x3 *m);
void m3x3_inverse(const struct m3x3 *src, struct m3x3 *dest);

#endif
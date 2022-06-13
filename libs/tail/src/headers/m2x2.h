#ifndef __TAIL_M2X2__
#define __TAIL_M2X2__

#include "m2x2.h"

struct m2x2 {
  float data[4];
};

void m2x2__create(
  float i0, float i2,
  float i1, float i3,
  struct m2x2 *m
);
float m2x2__determinant(const struct m2x2 *m);

#endif
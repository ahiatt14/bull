#ifndef __TAIL_M2X2__
#define __TAIL_M2X2__

#include "m2x2.h"

typedef struct M2X2 {
  float data[4];
} m2x2;

void m2x2_create(
  float i0, float i2,
  float i1, float i3,
  m2x2 *m
);
float m2x2_determinant(const m2x2 *m);

#endif
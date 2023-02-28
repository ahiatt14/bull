#ifndef __TAIL_M2X2__
#define __TAIL_M2X2__

#include "m2x2.h"

typedef struct TAILM2X2 {
  float data[4];
} M2x2;

void m2x2__create(
  float i0, float i2,
  float i1, float i3,
  M2x2 *const m
);
float m2x2__determinant(
  M2x2 const *const m
);

#endif
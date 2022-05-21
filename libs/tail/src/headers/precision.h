#ifndef __TAIL_PRECISION__
#define __TAIL_PRECISION__

typedef int (*flts_are_within_tolerance_ptr)(float a, float b, float tolerance);

int diff_is_within_tolerance(float a, float b, float tolerance);
int diff_is_within_mag_based_tolerance(float a, float b, float tolerance);

typedef struct FLOAT_TOLERANCE {
  float tolerance;
  flts_are_within_tolerance_ptr within_tolerance;
} float_tolerance;

#endif
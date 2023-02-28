#ifndef __TAIL_PRECISION__
#define __TAIL_PRECISION__

int diff_is_within_tolerance(
  float a,
  float b,
  float tolerance
);
int diff_is_within_mag_based_tolerance(
  float a,
  float b,
  float tolerance
);

typedef struct TAILFLOATTOLERANCE {
  float tolerance;
  int (*within_tolerance)(
    float a,
    float b,
    float tolerance
  );
} FloatTolerance;

#endif
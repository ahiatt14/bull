#ifndef __TAIL_VIEWPORT__
#define __TAIL_VIEWPORT__

typedef struct TAILVIEWPORT {
  float _width;
  float _height;
  short int _perspective_needs_recalculating;
} Viewport;

void viewport__set_width(
  int w,
  Viewport *v
);
void viewport__set_height(
  int h,
  Viewport *v
);
float viewport__get_aspect_ratio(
  Viewport const *const v
);
short int viewport__perspective_needs_recalculating(
  Viewport const *const v
);

#endif
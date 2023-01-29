#ifndef __TAIL_VIEWPORT__
#define __TAIL_VIEWPORT__

struct Viewport {
  float _width;
  float _height;
  short int _perspective_needs_recalculating;
};

void viewport__set_width(int w, struct Viewport *v);
void viewport__set_height(int h, struct Viewport *v);
float viewport__get_aspect_ratio(struct Viewport const *const v);
short int viewport__perspective_needs_recalculating(
  struct Viewport const *const v
);

#endif
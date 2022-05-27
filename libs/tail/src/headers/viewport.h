#ifndef __TAIL_VIEWPORT__
#define __TAIL_VIEWPORT__

struct viewport {
  float _width;
  float _height;
  short int _perspective_needs_recalculating;
};

void viewport__set_width(int w, struct viewport *v);
void viewport__set_height(int h, struct viewport *v);
float viewport__get_aspect_ratio(const struct viewport *v);
short int viewport__perspective_needs_recalculating(const struct viewport *v);

#endif
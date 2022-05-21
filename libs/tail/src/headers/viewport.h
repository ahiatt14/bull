#ifndef __TAIL_VIEWPORT__
#define __TAIL_VIEWPORT__

typedef struct VIEWPORT {
  float _width;
  float _height;
  short int _perspective_needs_recalculating;
} viewport;

void viewport__set_width(int w, viewport *v);
void viewport__set_height(int h, viewport *v);
float viewport__get_aspect_ratio(const viewport *v);
short int viewport__perspective_needs_recalculating(const viewport *v);

#endif
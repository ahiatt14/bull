#ifndef __BULL_MENU_SKY__
#define __BULL_MENU_SKY__

#include "tail.h"

void load_sky(const struct gpu_api *gpu);
void warp_sky_mesh(double delta_time, struct drawable_mesh *dm);
void draw_sky(
  const struct gpu_api *gpu,
  const struct m4x4 *view,
  const struct m4x4 *perspective
);

#endif
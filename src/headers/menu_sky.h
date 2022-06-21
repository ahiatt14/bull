#ifndef __BULL_MENU_SKY__
#define __BULL_MENU_SKY__

#include "tail.h"

void menu_sky__init(const struct gpu_api *gpu);
void menu_sky__tick(
  double delta_time,
  double seconds_since_creation,
  const struct gpu_api *gpu
);
void menu_sky__draw(
  const struct gpu_api *gpu,
  const struct m4x4 *view,
  const struct m4x4 *perspective
);

#endif
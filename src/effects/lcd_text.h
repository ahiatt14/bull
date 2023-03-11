#ifndef __BULL_LCD_TEXT__
#define __BULL_LCD_TEXT__

#include "tail.h"

#define MAX_READOUT_CHARACTERS 6

typedef struct BULLNUMBERREADOUT {
  Vec2 position;
  float scale;
  uint_fast16_t value;
} NumberReadout;

void lcd_text__copy_assets_to_gpu(
  GPU const *const gpu
);

void lcd_text__draw_number(
  NumberReadout readout,
  GPU const *const gpu
);

#endif
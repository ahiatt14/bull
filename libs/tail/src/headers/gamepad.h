#ifndef __TAIL_INPUT__
#define __TAIL_INPUT__

#include <stdint.h>

#include "vector.h"

#define BUTTON_A 0
#define BUTTON_B 1
#define BUTTON_X 2
#define BUTTON_Y 3
#define BUTTON_LEFT_BUMPER 4
#define BUTTON_RIGHT_BUMPER 5
#define BUTTON_SELECT 6
#define BUTTON_START 7
#define BUTTON_GUIDE 8
#define BUTTON_LEFT_THUMB_CLICK 9
#define BUTTON_RIGHT_THUMB_CLICK 10
#define BUTTON_DPAD_UP 11
#define BUTTON_DPAD_RIGHT 12
#define BUTTON_DPAD_DOWN 13
#define BUTTON_DPAD_LEFT 14

struct gamepad_input {
  struct vec2 left_stick_direction;
  float left_trigger;
  float right_trigger;
  uint16_t buttons;
  uint16_t previous_buttons;
};

uint8_t button_is_down(uint16_t button, uint16_t state);
uint8_t button_is_up(uint16_t button, uint16_t state);
uint8_t button_was_released(
  uint16_t button,
  uint16_t state,
  uint16_t previous_state
);

#endif
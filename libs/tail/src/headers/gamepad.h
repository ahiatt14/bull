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

typedef uint16_t gamepad_button_id;
typedef uint16_t gamepad_button_state;

typedef struct TAILGAMEPAD {
  Vec2 left_stick_direction;
  float left_trigger;
  float right_trigger;
  gamepad_button_state buttons;
  gamepad_button_state previous_buttons;
} Gamepad;

uint8_t button_is_down(
  gamepad_button_id button,
  Gamepad const *const gamepad
);
uint8_t button_is_up(
  gamepad_button_id button,
  Gamepad const *const gamepad
);
uint8_t button_was_released(
  gamepad_button_id button,
  Gamepad const *const gamepad
);

#endif
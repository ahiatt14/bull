#ifndef __TAIL_INPUT__
#define __TAIL_INPUT__

#include <stdint.h>

#include "vector.h"

struct gamepad_input {
  struct vec2 left_stick_direction;
  float right_trigger;
  float left_trigger; 
  uint8_t start_down;
  uint8_t select_down;
  uint8_t top_face_down;
  uint8_t left_face_down;
  uint8_t bottom_face_down;
  uint8_t right_face_down;
};

#endif
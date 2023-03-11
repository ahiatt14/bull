#include <math.h>

#include "tail.h"

#include "assets.h"
#include "constants.h"

#include "lcd_text.h"

#include "lcd_number_mesh.h"
#include "lcd_number_texture.h"
#include "flat_texture_frag.h"
#include "default_vert.h"

#define MESH_WIDTH 0.75f
#define CHAR_NDC_X_OFFSET 0.1f

static Shader number_readout_shader;

// TODO: add aspect ratio to position scaling
// TODO: compress mesh based on aspect ratio toooo?

void lcd_text__copy_asset_to_gpu(
  GPU const *const gpu
) {
  gpu->copy_static_mesh_to_gpu(&LCD_NUMBER_MESH);

  number_readout_shader.frag_src = FLAT_TEXTURE_FRAG_SRC;
  number_readout_shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&number_readout_shader);
}

static uint_fast8_t number_of_digits(
  uint_fast16_t value // TODO: make 32 bit eventually
) {
  if (value > 9999) {
    return 5;
  } else if (value > 999) {
    return 4;
  } else if (value > 99) {
    return 3;
  } else if (value > 9) {
    return 2;
  } else return 1;
}

static uint_fast8_t value_at_place(
  uint_fast8_t place,
  uint_fast16_t value
) {
  return (uint_fast8_t)(value / pow(10, place)) % 10;
}

void lcd_text__draw_number(
  NumberReadout readout,
  GPU const *const gpu
) {

  Transform first_digit_transform = {
    .position = (Vec3){ readout.position.x, readout.position.y, 0 },
    .scale = readout.scale,
    .rotation = quaternion__create(WORLDSPACE.up, 0)
  };

  static M4x4 model;

  gpu->select_shader(&number_readout_shader);
  gpu->select_texture(&LCD_NUMBER_TAIL_TEXTURE);

  for (int i = 0; i < number_of_digits(readout.value); i++) {

    first_digit_transform.position.x -=
      i * (MESH_WIDTH + CHAR_NDC_X_OFFSET) * readout.scale;

    uint_fast8_t digit = value_at_place(i, readout.value);

    space__create_model(
      &WORLDSPACE,
      &first_digit_transform,
      &model
    );
    gpu->set_shader_m4x4(&number_readout_shader, "model", &model);
    gpu->set_shader_vec2(
      &number_readout_shader,
      "uv_scroll",
      (Vec2){ (float)digit / 10.0f, 0 }
    );

    gpu->draw_mesh(&LCD_NUMBER_MESH);
    // TODO: we ain't gonna wanna do a draw call for every character
  }
}
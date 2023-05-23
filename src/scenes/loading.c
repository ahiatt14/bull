#include <stdio.h>

#include "tail.h"

#include "scene.h"

#include "assets.h"
#include "colors.h"

void loading__init(
  Window const *const window,
  Viewport *const vwprt,
  GPU const *const gpu
) {
  gpu->copy_texture_to_gpu(
    FILTER__NEAREST,
    WRAP__REPEAT,
    TEXTURES[0]
  );
}

void loading__tick(
  GameTime time,
  Window const *const window,
  Viewport *const vwprt,
  GPU const *const gpu,
  uint8_t previous_scene,
  void (*switch_scene)(uint8_t new_scene)
) {

  static uint_fast8_t textures_copied = 1;

  if (textures_copied < TEXTURE_COUNT) {
    gpu->copy_texture_to_gpu(
      FILTER__NEAREST,
      WRAP__REPEAT,
      TEXTURES[textures_copied]
    );
    textures_copied++;
  }

  if (textures_copied == TEXTURE_COUNT)
    switch_scene(SCENE__ACTION);

  gpu->clear(&COLOR_BLUE);
}
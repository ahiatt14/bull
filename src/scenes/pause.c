#include "tail.h"

#include "ecs.h"

#include "scene.h"

#include "constants.h"
#include "assets.h"

void pause__init(
  Window const *const window,
  Viewport *const vwprt,
  GPU const *const gpu
) {

}

void pause__tick(
  GameTime time,
  Window const *const window,
  Viewport *const vwprt,
  GPU const *const gpu,
  uint8_t previous_scene,
  void (*switch_scene)(uint8_t new_scene)
) {
  
}
#ifndef __BULL_LAUNCHERS__
#define __BULL_LAUNCHERS__

#include "tail.h"

#include "ecs.h"

EntityId create_arrow_launcher(
  struct Vec3 position,
  struct ECS *const ecs
) {

  EntityId arrow_launcher = ecs__create_entity(ecs);

  ecs__add_transform(

  );
  ecs__add_uv_scroll(
    arrow_launcher,
    (struct )
    ecs
  );
  ecs__add_alpha_effect(arrow_launcher, ecs);
  ecs__add_draw(

  );
  ecs__add_radius(
    
  );
}

#endif
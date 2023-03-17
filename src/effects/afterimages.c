#include "tail.h"

#include "ecs.h"

#include "afterimages.h"

#include "constants.h"

#include "rex_mesh.h"
#include "default_vert.h"
#include "afterimage_frag.h"

static Shader player_afterimage_shader;

static void destroy_afterimage(
  EntityId afterimage,
  Seconds remainder,
  ECS *const ecs
) {
  ecs__mark_for_destruction(afterimage, ecs);
}

void afterimages__copy_assets_to_gpu(
  GPU const *const gpu
) {

  player_afterimage_shader.frag_src = AFTERIMAGE_FRAG_SRC;
  player_afterimage_shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&player_afterimage_shader);

  gpu->copy_static_mesh_to_gpu(&REX_MESH);
}

static void draw_afterimage(
  GameTime time,
  Camera const *const camera,
  GPU const *const gpu,
  Transform const *const hierarchy_transform,
  EntityId id,
  ECS const *const ecs
) {

  gpu->set_shader_vec3(
    ecs->entities[id].draw.shader,
    "afterimage_pos_worldspace",
    ecs->entities[id].transform.position
  );
  gpu->set_shader_vec3(
    ecs->entities[id].draw.shader,
    "color",
    COLOR_AQUA_BLUE
  );

  ecs__draw_mesh(
    time,
    camera,
    gpu,
    hierarchy_transform,
    id,
    ecs
  );
}

// TODO: can easily be made reusable,
// pass in params like timeout limit
// shader, and draw callback
void create_player_afterimage(
  EntityId player,
  Seconds remainder,
  ECS *const ecs
) {
  EntityId afterimage = ecs__create_entity(ecs);
  ecs__add_transform(
    afterimage,
    ecs->entities[player].transform,
    ecs
  );
  ecs__add_alpha_effect(afterimage, ecs);
  ecs__add_timeout(
    afterimage,
    (Timeout){
      .age = remainder,
      .limit = 0.2f,
      .on_timeout = destroy_afterimage
    },
    ecs
  );
  ecs__add_draw(
    afterimage,
    (Draw){
      .mesh = &REX_MESH,
      .draw = draw_afterimage,
      .shader = &player_afterimage_shader,
      .textures = 0
    },
    ecs
  );
}
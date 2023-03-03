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
  EntityId id,
  ECS const *const ecs
) {

  static Entity const *afterimage;
  afterimage = &ecs->entities[id];

  static M4x4 model;
  static M3x3 normals_model;
  static Shader *shader;

  shader = afterimage->draw.shader;

  space__create_model(&WORLDSPACE, &afterimage->transform, &model);
  gpu->set_shader_m4x4(shader, "model", &model);
  space__create_normals_model(&model, &normals_model);
  gpu->set_shader_m3x3(shader, "normals_model", &normals_model);

  gpu->set_shader_vec3(
    shader,
    "afterimage_pos_worldspace",
    afterimage->transform.position
  );

  gpu->set_shader_vec3(shader, "color", COLOR_AQUA_BLUE);
  // TODO: do a distance from player trajectory line fade thing?

  gpu->draw_mesh(afterimage->draw.mesh);
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
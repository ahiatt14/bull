#include "tail.h"

#include "ecs.h"

#include "afterimages.h"

#include "colors.h"

#include "player_body_mesh.h"
#include "player_left_leg_mesh.h"
#include "player_right_leg_mesh.h"

#include "default_vert.h"
#include "afterimage_frag.h"

static Shader afterimage_shader;

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

  afterimage_shader.frag_src = AFTERIMAGE_FRAG_SRC;
  afterimage_shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&afterimage_shader);

  // gpu->copy_static_mesh_to_gpu(&REX_MESH);
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
    COLOR_MOONLIGHT
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

void create_afterimage(
  EntityId entity,
  Seconds remainder,
  ECS *const ecs
) {

  EntityId afterimage = ecs__create_entity(ecs);
  ecs__add_transform(
    afterimage,
    ecs->entities[entity].transform,
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
      .mesh = ecs->entities[entity].draw.mesh,
      .draw = draw_afterimage,
      .shader = &afterimage_shader,
      .textures = 0
    },
    ecs
  );

  // EntityId child_part, child_part_afterimage;
  // for (
  //   uint_fast8_t i = 0;
  //   i < ecs->entities[player].hierarchy.child_count;
  //   i++
  // ) {
  //   child_part = ecs->entities[player].hierarchy.children[i];
  //   child_part_afterimage = ecs__create_entity(ecs);

  //   ecs__add_parent_relationship(
  //     child_part_afterimage,
  //     core_afterimage,
  //     ecs
  //   );
  //   ecs__add_transform(
  //     child_part_afterimage,
  //     ecs->entities[child_part].transform,
  //     ecs
  //   );
  //   ecs__add_alpha_effect(child_part_afterimage, ecs);
  //   // ecs__add_timeout(
  //   //   child_part_afterimage,
  //   //   (Timeout){
  //   //     .age = remainder,
  //   //     .limit = 0.19f,
  //   //     .on_timeout = destroy_afterimage
  //   //   },
  //   //   ecs
  //   // );
  //   ecs__add_draw(
  //     child_part_afterimage,
  //     (Draw){
  //       .mesh = ecs->entities[child_part].draw.mesh,
  //       .draw = draw_afterimage,
  //       .shader = &afterimage_shader,
  //       .textures = 0
  //     },
  //     ecs
  //   );
  // }
}
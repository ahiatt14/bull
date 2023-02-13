#include <math.h>
#include <stdlib.h>

#include "tail.h"

#include "ecs.h"

#include "explosions.h"
#include "sparks.h"

#include "constants.h"

#include "billboard_geo.h"
#include "billboard_vert.h"
#include "default_vert.h"
#include "lowpoly_sphere_flat_mesh.h"
#include "fireball_texture.h"
#include "blink_texture.h"
#include "explosion_blink_frag.h"
#include "explosion_frag.h"

static struct Shader rpg_explosion_blink_shader;
static struct Shader rpg_explosion_fireball_shader;

static void destroy_explosion(
  EntityId explosion,
  Seconds remainder,
  struct ECS *const ecs
) {
  ecs__mark_for_destruction(explosion, ecs);
}

// struct Quaternion quaternion__inverse(
//   struct Quaternion q
// ) {
//   struct Quaternion conjugate = {
//     .v = vec3__negate(q.v),
//     .w = q.w
//   };
//   return quaternion__multiply(conjugate, q);
// }

void explosions__copy_assets_to_gpu(
  struct GPU const *const gpu
) {

  gpu->copy_static_mesh_to_gpu(&LOWPOLY_SPHERE_FLAT_MESH);

  rpg_explosion_blink_shader.frag_src = EXPLOSION_BLINK_FRAG_SRC;
  rpg_explosion_blink_shader.geo_src= BILLBOARD_GEO_SRC;
  rpg_explosion_blink_shader.vert_src = BILLBOARD_VERT_SRC;
  gpu->copy_shader_to_gpu(&rpg_explosion_blink_shader);

  rpg_explosion_fireball_shader.frag_src = EXPLOSION_FRAG_SRC;
  rpg_explosion_fireball_shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&rpg_explosion_fireball_shader);

  gpu->copy_texture_to_gpu(&BLINK_TEXTURE);
  gpu->copy_texture_to_gpu(&FIREBALL_TEXTURE);
}

void create_rpg_explosion(
  EntityId rocket,
  struct Vec3 camera_position,
  struct ECS *const ecs
) {

  EntityId blink = ecs__create_entity(ecs);

  struct Vec3 blink_position = vec3_plus_vec3(
    ecs->entities[rocket].transform.position,
    scalar_x_vec3(
      0.3f,
      vec3_minus_vec3(
        camera_position,
        ecs->entities[rocket].transform.position
      )
    )
  );

  ecs__add_transform(
    blink,
    (struct Transform){
      .position = blink_position,
      .scale = 0.5f
    },
    ecs
  );
  ecs__add_timeout(
    blink,
    (struct Timeout){
      .age = 0,
      .limit = 1.0f / 15.0f,
      .on_timeout = destroy_explosion
    },
    ecs
  );
  ecs__add_draw_billboard(
    blink,
    (struct Draw){
      .texture = &BLINK_TEXTURE,
      .shader = &rpg_explosion_blink_shader
      // .shader = &FLAT_TEXTURE_SHADER
    },
    ecs
  );

  EntityId fireball = ecs__create_entity(ecs);

  ecs__add_transform(
    fireball,
    (struct Transform){
      .position = ecs->entities[rocket].transform.position,
      .rotation = ecs->entities[rocket].transform.rotation,
      .scale = 0.5f
    },
    ecs
  );
  ecs__add_timeout(
    fireball,
    (struct Timeout){
      .age = 0,
      .limit = 0.5f,
      .on_timeout = destroy_explosion
    },
    ecs
  );
  ecs__add_velocity(
    fireball,
    vec3__normalize(ecs->entities[rocket].velocity),
    ecs
  );
  ecs__add_uv_scroll(
    fireball,
    (struct Vec2){
      0.5f - rand() / (float)RAND_MAX,
      rand() / (float)RAND_MAX
    },
    ecs
  );
  ecs__add_draw(
    fireball,
    (struct Draw){
      .texture = &FIREBALL_TEXTURE,
      .shader = &rpg_explosion_fireball_shader,
      .mesh = &LOWPOLY_SPHERE_FLAT_MESH
    },
    ecs
  );

  create_sparks(
    ecs->entities[rocket].transform.position,
    ecs->entities[rocket].velocity,
    5,
    ecs
  );
}
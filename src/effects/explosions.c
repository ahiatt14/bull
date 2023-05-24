// #include <math.h>
// #include <stdlib.h>

// #include "tail.h"

// #include "ecs.h"

// #include "explosions.h"
// #include "sparks.h"

// #include "assets.h"
// #include "constants.h"

// #include "billboard_geo.h"
// #include "billboard_vert.h"
// #include "default_vert.h"
// #include "explosion_blink_frag.h"
// #include "explosion_frag.h"

// static Shader rpg_explosion_blink_shader;
// static Shader rpg_explosion_fireball_shader;

// static void destroy_explosion(
//   EntityId explosion,
//   Seconds remainder,
//   ECS *const ecs
// ) {
//   ecs__mark_for_destruction(explosion, ecs);
// }

// void explosions__copy_assets_to_gpu(
//   GPU const *const gpu
// ) {

//   rpg_explosion_blink_shader.frag_src = EXPLOSION_BLINK_FRAG_SRC;
//   rpg_explosion_blink_shader.geo_src= BILLBOARD_GEO_SRC;
//   rpg_explosion_blink_shader.vert_src = BILLBOARD_VERT_SRC;
//   gpu->copy_shader_to_gpu(&rpg_explosion_blink_shader);

//   rpg_explosion_fireball_shader.frag_src = EXPLOSION_FRAG_SRC;
//   rpg_explosion_fireball_shader.vert_src = DEFAULT_VERT_SRC;
//   gpu->copy_shader_to_gpu(&rpg_explosion_fireball_shader);
// }

// void create_rpg_explosion(
//   EntityId rocket,
//   Vec3 camera_position,
//   ECS *const ecs
// ) {

//   EntityId blink = ecs__create_entity(ecs);

//   Vec3 blink_position = vec3_plus_vec3(
//     ecs->entities[rocket].transform.position,
//     scalar_x_vec3(
//       0.3f,
//       vec3_minus_vec3(
//         camera_position,
//         ecs->entities[rocket].transform.position
//       )
//     )
//   );

//   ecs__add_transform(
//     blink,
//     (Transform){
//       .position = blink_position,
//       .scale = 0.5f
//     },
//     ecs
//   );
//   ecs__add_timeout(
//     blink,
//     (Timeout){
//       .age = 0,
//       .limit = 1.0f / 30.0f,
//       .on_timeout = destroy_explosion
//     },
//     ecs
//   );
//   ecs__add_draw(
//     blink,
//     (Draw){
//       .textures = BLINK_TEXTURE,
//       .shader = &rpg_explosion_blink_shader,
//       .draw = ecs__draw_billboard
//     },
//     ecs
//   );

//   EntityId fireball = ecs__create_entity(ecs);

//   ecs__add_transform(
//     fireball,
//     (Transform){
//       .position = ecs->entities[rocket].transform.position,
//       .rotation = ecs->entities[rocket].transform.rotation,
//       .scale = 0.5f
//     },
//     ecs
//   );
//   ecs__add_timeout(
//     fireball,
//     (Timeout){
//       .age = 0,
//       .limit = 0.5f,
//       .on_timeout = destroy_explosion
//     },
//     ecs
//   );
//   ecs__add_velocity(
//     fireball,
//     vec3__normalize(ecs->entities[rocket].velocity),
//     ecs
//   );
//   ecs__add_uv_scroll(
//     fireball,
//     (ScrollUV){
//       .speed = (Vec2){
//         0.5f - rand() / (float)RAND_MAX,
//         rand() / (float)RAND_MAX
//       },
//       .total = (Vec2){0}
//     },
//     ecs
//   );
//   ecs__add_draw(
//     fireball,
//     (Draw){
//       .textures = FIREBALL_TEXTURE,
//       .shader = &rpg_explosion_fireball_shader,
//       .mesh = &LOWPOLY_SPHERE_FLAT_MESH,
//       .draw = ecs__draw_mesh
//     },
//     ecs
//   );

//   create_sparks(
//     ecs->entities[rocket].transform.position,
//     ecs->entities[rocket].velocity,
//     5,
//     ecs
//   );
// }
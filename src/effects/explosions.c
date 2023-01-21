#include <stdio.h>

#include "tail.h"

#include "explosions.h"
#include "billboard.h"

#include "constants.h"
#include "tail_helpers.h"
#include "bull_math.h"

#include "sphere_mesh.h"
#include "lowpoly_mushroom_cloud_mesh.h"
#include "mushroom_cloud_frag.h"
#include "explosion_frag.h"
#include "default_vert.h"
#include "fireball_texture.h"
#include "blink_texture.h"

#define BLINK_SCALE 2.0f

#define BLINK_LIFESPAN_SECONDS 0.033f
#define EXPLOSION_LIFESPAN_SECONDS 0.5f
#define MUSHROOM_CLOUD_LIFESPAN_SECONDS 1.0f

#define BILLOW_WORLD_UNIT_PER_SEC 1.0f

static void scroll_mushroom_cloud_uvs(
  struct gametime time,
  struct drawable_mesh *const mushroom_cloud_mesh
);

static struct explosion* get_explosion(
  uint_fast16_t from_start,
  struct explosions *const explos
) {
  return &(explos->_ring_buffer[
    (explos->_tail + from_start) % MAX_EXPLOSIONS
  ]);
}

static struct shader mushroom_cloud_shader;
static struct shader explosion_shader;

void explosions__copy_assets_to_gpu(
  struct gpu_api const *const gpu
) {
  gpu->copy_static_mesh_to_gpu(&sphere_mesh);
  gpu->copy_dynamic_mesh_to_gpu(&lowpoly_mushroom_cloud_mesh);

  mushroom_cloud_shader.frag_src = mushroom_cloud_frag_src;
  mushroom_cloud_shader.vert_src = default_vert_src;
  gpu->copy_shader_to_gpu(&mushroom_cloud_shader);
  explosion_shader.frag_src = explosion_frag_src;
  explosion_shader.vert_src = default_vert_src;
  gpu->copy_shader_to_gpu(&explosion_shader);

  gpu->copy_texture_to_gpu(&fireball_texture);
  gpu->copy_texture_to_gpu(&blink_texture);
}

void explosions__create(
  struct vec3 position,
  struct explosions *const explos
) {

  static struct explosion explo;
  explo = (struct explosion){
    .mushroom_cloud_transform = (struct transform){
      .position = position,
      .scale = 1
    },
    .explosion_transform = (struct transform){
      .scale = 0.7f,
      .position = position
    },
    .blink = (struct billboard){
      .texture = &fireball_texture,
      .transform = (struct transform){
        .scale = 1,
        .position = position
      }
    },
    .sec_since_activation = 0
  };

  if (explos->_active_count == MAX_EXPLOSIONS) {
    explos->_ring_buffer[explos->_tail] = explo;
    explos->_tail = (explos->_tail + 1) % MAX_EXPLOSIONS;
  } else {
    explos->_ring_buffer[
      (explos->_tail + explos->_active_count) % MAX_EXPLOSIONS
    ] = explo;
    explos->_active_count++;
  }
}

void explosions__destroy(
  uint_fast16_t from_start,
  struct explosions *const explos
) {
  uint_fast16_t index_to_replace =
    (explos->_tail + from_start) % MAX_EXPLOSIONS;
  explos->_ring_buffer[index_to_replace] =
    explos->_ring_buffer[explos->_tail];
  explos->_tail = (explos->_tail + 1) % MAX_EXPLOSIONS;
  explos->_active_count--;
}

void explosions__destroy_all(
  struct explosions *const explos
) {
  explos->_tail = 0;
  explos->_active_count = 0;
}

void explosions__update(
  struct gametime time,
  struct explosions *const explos
) {
  static uint_fast16_t from_start_offsets_to_destroy[MAX_EXPLOSIONS];
  static uint_fast8_t from_start_offsets_to_destroy_offset;
  from_start_offsets_to_destroy_offset = 0;

  for (int i = 0; i < explos->_active_count; i++) {

    static struct explosion *explo;
    explo = get_explosion(i, explos);
    explo->sec_since_activation += time.delta;
    explo->mushroom_cloud_transform.position.y +=
      BILLOW_WORLD_UNIT_PER_SEC * time.delta;

    if (explo->sec_since_activation >= MUSHROOM_CLOUD_LIFESPAN_SECONDS) {
      from_start_offsets_to_destroy[
        from_start_offsets_to_destroy_offset++
      ] = i;
    }
  }

  for (int i = 0; i < from_start_offsets_to_destroy_offset; i++) {
    explosions__destroy(from_start_offsets_to_destroy[i], explos);
  }

  scroll_mushroom_cloud_uvs(time, &lowpoly_mushroom_cloud_mesh);
}

void explosions__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct explosions *const explos
) {

  gpu->update_gpu_mesh_data(&lowpoly_mushroom_cloud_mesh);

  for (int i = 0; i < explos->_active_count; i++) {

    static struct m4x4 local_to_world;

    static struct explosion *explo;
    explo = get_explosion(i, explos);

    // if (explo->sec_since_activation < BLINK_LIFESPAN_SECONDS)
      billboard__draw(cam, gpu, &explo->blink);

    gpu->select_shader(&mushroom_cloud_shader);
    gpu->select_texture(&fireball_texture);

    gpu->set_shader_float(
      &mushroom_cloud_shader,
      "total_elapsed_seconds",
      explo->sec_since_activation
    );

    gpu->set_shader_float(
      &mushroom_cloud_shader,
      "lifespan_in_seconds",
      MUSHROOM_CLOUD_LIFESPAN_SECONDS
    );
    
    m4x4__translation(
      explo->mushroom_cloud_transform.position,
      &local_to_world
    );  
    gpu__set_mvp(
      &local_to_world,
      &M3X3_IDENTITY,
      cam,
      &mushroom_cloud_shader,
      gpu
    );
    gpu->draw_mesh(&lowpoly_mushroom_cloud_mesh);

    gpu->select_shader(&explosion_shader);
    gpu->select_texture(&fireball_texture);

    gpu->set_shader_float(
      &explosion_shader,
      "total_elapsed_seconds",
      explo->sec_since_activation
    );

    gpu->set_shader_float(
      &explosion_shader,
      "lifespan_in_seconds",
      EXPLOSION_LIFESPAN_SECONDS
    );

    space__create_model(
      &WORLDSPACE,
      &(explo->explosion_transform),
      &local_to_world
    );
    gpu__set_mvp(
      &local_to_world,
      &M3X3_IDENTITY,
      cam,
      &explosion_shader,
      gpu
    );
    // gpu->draw_mesh(&sphere_mesh);
  }
}

static void scroll_mushroom_cloud_uvs(
  struct gametime time,
  struct drawable_mesh *const mushroom_cloud_mesh
) {
  for (int i = 0; i < mushroom_cloud_mesh->vertices_length; i++) {
    mushroom_cloud_mesh->vertices[i].uv.y += time.delta * 0.35f;
  }
}
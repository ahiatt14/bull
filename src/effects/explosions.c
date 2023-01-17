#include <stdio.h>

#include "tail.h"

#include "explosions.h"

#include "constants.h"
#include "tail_helpers.h"
#include "bull_math.h"

#include "lowpoly_mushroom_cloud_mesh.h"
#include "mushroom_cloud_frag.h"
#include "default_vert.h"
#include "fireball_texture.h"

#define EXPLOSION_LIFESPAN_SECONDS 1.0f

static struct explosion* get_explosion(
  uint_fast16_t from_start,
  struct explosions *const explos
) {
  return &(explos->_ring_buffer[(explos->_tail + from_start) % MAX_EXPLOSIONS]);
}

static struct shader mushroom_cloud_shader;

void explosions__copy_assets_to_gpu(
  struct gpu_api const *const gpu
) {
  gpu->copy_static_mesh_to_gpu(&lowpoly_mushroom_cloud_mesh);
  mushroom_cloud_shader.frag_src = mushroom_cloud_frag_src;
  mushroom_cloud_shader.vert_src = default_vert_src;
  gpu->copy_shader_to_gpu(&mushroom_cloud_shader);
  gpu->copy_texture_to_gpu(&fireball_texture);
}

void explosions__create(
  struct battlefield_pos bfpos,
  struct explosions *const explos
) {
  if (explos->_active_count == MAX_EXPLOSIONS) {
    explos->_ring_buffer[explos->_tail] = (struct explosion){
      .position = bfpos,
      .sec_since_activation = 0
    };
    explos->_tail = (explos->_tail + 1) % MAX_EXPLOSIONS;
  } else {
    explos->_ring_buffer[
      (explos->_tail + explos->_active_count) % MAX_EXPLOSIONS
    ] = (struct explosion){
      .position = bfpos,
      .sec_since_activation = 0
    };
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

    if (explo->sec_since_activation >= EXPLOSION_LIFESPAN_SECONDS) {
      from_start_offsets_to_destroy[from_start_offsets_to_destroy_offset++] = i;
    }
  }

  for (int i = 0; i < from_start_offsets_to_destroy_offset; i++) {
    explosions__destroy(from_start_offsets_to_destroy[i], explos);
  }
}

void explosions__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct explosions *const explos
) {

  gpu->select_shader(&FLAT_TEXTURE_SHADER);
  gpu->select_texture(&fireball_texture);

  for (int i = 0; i < explos->_active_count; i++) {

    static struct explosion *explo;
    explo = get_explosion(i, explos);
    
    static struct m4x4 local_to_world;
    m4x4__translation(
      battlefield_to_world_pos(explo->position),
      &local_to_world
    );  
    gpu__set_mvp(
      &local_to_world,
      &M3X3_IDENTITY,
      cam,
      &FLAT_TEXTURE_SHADER,
      gpu
    );
    gpu->draw_mesh(&lowpoly_mushroom_cloud_mesh);
  }
}
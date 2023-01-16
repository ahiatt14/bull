#include "tail.h"

#include "explosions.h"

void explosions__copy_assets_to_gpu(
  struct gpu_api const *const gpu
) {

}

void explosions__create(
  struct vec3 position,
  struct explosions *const explos
) {
  if (explos->_active_count == MAX_EXPLOSIONS) {
    explos->_ring_buffer[explos->_tail] = (struct fireball){
      .position = bfpos,
      .sec_since_activation = 0
    };
    explos->_tail = (explos->_tail + 1) % MAX_EXPLOSIONS;
  } else {
    explos->_ring_buffer[
      (explos->_tail + explos->_active_count) % MAX_EXPLOSIONS
    ] = (struct fireball){
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
  uint_fast16_t index_to_replace = (explos->_tail + from_start) % MAX_EXPLOSIONS;
  explos->_ring_buffer[index_to_replace] = explos->_ring_buffer[explos->_tail];
  explos->_tail = (explos->_tail + 1) % MAX_EXPLOSIONS;
  explos->_active_count--;
}

void explosions__destroy_all(
  struct explosions *const explos
) {
  explos->_tail = 0;
  explos->_count = 0;
}

void explosions__update(
  struct gametime time,
  struct explosions *const explos
) {

}

void explosions__draw(
  struct gpu_api const *const gpu,
  struct explosions const *const explos
) {

}
static void bring_out_the_arm() {

}

static void send_the_arm_home() {

}

struct collision_check {
  void (*check_overlaps)(
    void (*handle_enter_overlap)(),
    void (*handle_leave_overlap)(),
    struct player
  );
  void (*handle_enter_overlap)();
  void (*handle_leave_overlap)();
};

static struct collision_check collision_checks[COLLISION_CHECK_COUNT] = {
  (struct collision_check){
    check_bullet_overlaps,
    handle_bullet_enter,
    handle_bullet_leave
  },
  (struct collision_check){
    check_hard_block_overlaps,
    handle_hard_block_enter,
    handle_hard_block_leave
  }
};

action__init() {
  
}

action__tick() {

  handle_player_input(
    player,
    player_action_callbacks,
    arena,
    player_arena_callbacks
  );

  // update_bullets();

  for (int i = 0; i < COLLISION_CHECK_COUNT; i++) {
    collision_checks[i].check_overlaps(
      playr,
      collision_checks[i].handle_enter_overlap,
      collision_checks[i].handle_leave_overlap
    );
    if (
      player.current_input_state == PLAYER_STATE__REELING ||
      player.current_input_state == PLAYER_STATE__DYING
    ) break;
  }


}

struct player_action_callbacks {
  void (*start_main_auto_fire)();
  void (*stop_main_auto_fire)();
  void (*fire_special)();
};
struct player_arena_callbacks {
  void (*player_entered_core)();
  void (*player_left_arena)();
  void (*player_entered_arena)();
};

static void move_player(
  double delta,
  struct gamepad_input const *const gamepad,
  struct player_arena_callbacks const *const callbacks,
  struct *const player,
  struct *const arena
) {
  player_states[player->current_state]->update(
    delta,
    gamepad
  );
}




  background_shader.frag_src = flat_texture_frag_src;
  background_shader.vert_src = default_vert_src;
  gpu->copy_shader_to_gpu(&background_shader);
  gpu->copy_static_mesh_to_gpu(&background_mesh);
  gpu->copy_texture_to_gpu(&clouds_texture);

  static struct drawable_mesh background_mesh = {
  .vertices = (struct vertex [4]){
    {{ -1, -1, 0 },{ 0, 0, 1 },{ 0, 0 }},
    {{ 1, -1, 0 },{ 0, 0, 1 },{ 1, 0 }},
    {{ 1, 1, 0 },{ 0, 0, 1 },{ 1, 1 }},
    {{ -1, 1, 0 },{ 0, 0, 1 },{ 0, 1 }}
  },
  .indices = (unsigned int[6]){
    0, 1, 2,
    0, 2, 3
  },
  .vertices_size = sizeof(struct vertex) * 4,
  .indices_size = sizeof(unsigned int) * 6,
  .indices_length = 6
};
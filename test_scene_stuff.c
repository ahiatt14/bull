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

action__tick() {


  move_player(
    player,
    arena,
    player_arena_callbacks
  );

  update_bullets();
  update_lasers();
  update_blocks();
  update_arms(); 

  // e.g.
  for (int i = 0; i < COLLISION_CHECK_COUNT; i++) {
    collision_checks[i].check_overlaps(
      playr,
      collision_checks[i].handle_enter_overlap,
      collision_checks[i].handle_leave_overlap
    );
    if (
      player.current_input_state == PLAYER_STATE__REELING ||
      player.current_effect_state == PLAYER_STATE__DYING
    ) break;
  }


}

struct player_arena_callbacks {
  void (*handle_player_entering_core)()
  void (*handle_player_leaving_arena)();
  void (*handle_player_entering_arena)();
};

static void move_player(
  double delta_time,
  struct gamepad_input const *const gamepad,
  struct player_arena_callbacks const *const callbacks,
  struct *const player,
  struct *const arena
) {
  player_states[player->current_state]->update(
    delta_time,
    gamepad
  );
}


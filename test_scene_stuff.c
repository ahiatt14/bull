static void bring_out_the_arm() {

}

static void send_the_arm_home() {

}

// TODO: wonder if we want to handle entering/leaving for "bullets"
// or small items. double the checks could be expensive and we'll probably
// have lotsa bullets?
static struct collision_check collision_checks[COLLISION_CHECK_COUNT] = {
  (struct collision_check){
    check_bullet_overlaps,
    handle_bullet_enter_overlap,
    handle_bullet_leave_overlap
  },
  (struct collision_check){
    check_laser_overlaps,
    handle_
  }
};

action__tick() {


  move_player(
    player,
    arena,
    bring_out_the_arm,
    send_the_arm_home
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
      player.current_state == PLAYER_STATE__REELING ||
      player.current_state == PLAYER_STATE__DYING
    ) break;
  }


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

static void move_player(
  struct player,
  struct arena,
  void (*handle_player_leaving_arena)()
  void (*handle_player_entering_arena)()
) {

}
// character.h

#define MAX_HORNETS 20
#define MAX_BOMBERS 10

#define MAX_CHARACTERS MAX_HORNETS+MAX_BOMBERS

struct blackboard {

};

struct character {
  struct transform transform;
  struct battlefield_pos previous_position;
  struct battlefield_pos projected_position;
  void (*update)(
    struct GameTime time,
    struct blackboard const* const bboard,
    struct character *const chr
  );
  uint_fast16_t state_index;
};

// hornet.c

struct hornet_state {

};

void hornet__entering_battlefield_udpate(
  struct GameTime time,
  struct blackboard const* const bboard,
  struct character *const chr
) {

}

void hornet__entering_battlefield_update(

) {
  
}
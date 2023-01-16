#version 330 core

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;
out vec4 FragColor;

const float MAX_PLAYER_TO_FRAG_DISTANCE = 3;
const float MISSILE_GUIDE_HALF_WIDTH = 2;
const float LINE_THICKNESS = 0.1;

vec2 WORLD_ORIGIN = vec2(0);

uniform vec3 player_pos_worldspace;
uniform float arena_radius_worldspace;

uniform vec3 color = vec3(0.20, 1.00, 0.33);

void main() {

  vec2 player_to_origin = WORLD_ORIGIN - player_pos_worldspace.xz;
  vec2 player_to_origin_perpendicular = vec2(player_to_origin.y, -player_to_origin.x);
  vec2 frag_pos_to_origin = WORLD_ORIGIN - fs_in.world_frag_pos.xz;
  float frag_distance_to_player_origin_line =
    abs(dot(normalize(player_to_origin_perpendicular), frag_pos_to_origin));

  float frag_radius_world = distance(
    fs_in.world_frag_pos.xz,
    WORLD_ORIGIN
  );

  bool player_far_enough_away =
    distance(fs_in.world_frag_pos, player_pos_worldspace) >=
    MAX_PLAYER_TO_FRAG_DISTANCE;

  bool show_missile_guide =
    frag_distance_to_player_origin_line >
    MISSILE_GUIDE_HALF_WIDTH &&
    frag_distance_to_player_origin_line <
    (MISSILE_GUIDE_HALF_WIDTH + LINE_THICKNESS) &&
    frag_radius_world <= arena_radius_worldspace;

  bool show_radius_guide =
    abs(frag_radius_world - arena_radius_worldspace) < 0.05 &&
    frag_distance_to_player_origin_line > MISSILE_GUIDE_HALF_WIDTH * 2.5;

  bool show_center_guide = frag_radius_world < 0.1;

  // TOOD: this stinks lol
  bool show_guide =
    show_center_guide ||
    (player_far_enough_away && (show_missile_guide || show_radius_guide));
  
  FragColor = vec4(
    color,
    show_guide ? 0.4 : 0.0
  );
}
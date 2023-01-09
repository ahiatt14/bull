#version 330 core

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;
out vec4 FragColor;

const float MISSILE_GUIDE_HALF_WIDTH = 1.2;
const float LINE_THICKNESS = 0.1;

vec2 WORLD_ORIGIN = vec2(0);

uniform vec3 player_pos_worldspace;
uniform float arena_radius_worldspace;

uniform vec3 color = vec3(0.20, 1.00, 0.33);

float distance_to_player_origin_line() {
  vec2 player_to_origin = WORLD_ORIGIN - player_pos_worldspace.xz;
  vec2 perpendicular = vec2(player_to_origin.y, -player_to_origin.x);
  vec2 frag_pos_to_origin = WORLD_ORIGIN - fs_in.world_frag_pos.xz;
  return abs(dot(normalize(perpendicular), frag_pos_to_origin));
}

void main() {

  float frag_distance_to_player_origin_line =
    distance_to_player_origin_line();

  float frag_radius_world = distance(
    fs_in.world_frag_pos.xz,
    WORLD_ORIGIN
  );

  bool show_missile_guide =
    frag_distance_to_player_origin_line >
    MISSILE_GUIDE_HALF_WIDTH &&
    frag_distance_to_player_origin_line <
    (MISSILE_GUIDE_HALF_WIDTH + LINE_THICKNESS) &&
    frag_radius_world <= arena_radius_worldspace;

  bool show_radius_guide =
    abs(frag_radius_world - arena_radius_worldspace) < 0.05 &&
    frag_distance_to_player_origin_line > MISSILE_GUIDE_HALF_WIDTH;

  bool show_guide = show_missile_guide || show_radius_guide;
  
  FragColor = vec4(
    color,
    show_guide ? 0.4 : 0.0
  );
}
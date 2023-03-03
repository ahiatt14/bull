#version 330 core

// const vec2 WORLD_ORIGIN = vec2(0);

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;

uniform vec3 color;

uniform float seconds_since_activation;
uniform float limit_in_seconds;

// uniform vec3 afterimage_pos_worldspace;

out vec4 FragColor;

void main() {

  // vec2 trajectory = WORLD_ORIGIN - afterimage_pos_worldspace.xz;
  // vec2 trajectory_perpendicular =
  //   vec2(trajectory.y, -trajectory.x);
  // vec2 frag_pos_to_origin = WORLD_ORIGIN - fs_in.world_frag_pos.xz;
  // float frag_distance_to_trajectory =
  //   abs(dot(
  //     normalize(trajectory_perpendicular),
  //     frag_pos_to_origin
  //   ));

  float ratio = seconds_since_activation / limit_in_seconds;

  // float trajectory_alpha = smoothstep(
  //   0,
  //   0.5,
  //   frag_distance_to_trajectory - ratio
  // );

  FragColor = vec4(
    mix(color, vec3(1), ratio),
    (1.0 - ratio)
  );
}
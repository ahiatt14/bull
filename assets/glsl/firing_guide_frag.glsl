#version 330 core

uniform float player_radius = 1;
float radius = 0;
float alpha = 0;

uniform vec3 color = vec3(
  1,
  0,
  0
);

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;

out vec4 FragColor;

void main() {

  // TODO: should convert player pos to screen coordinates
  // and compare against FragCoord.xy?
  if (
    distance(
      fs_in.world_frag_pos,
      vec3(0, 0, 0)
    ) < 0.05
  ) {
    alpha = 0.6;
  } else {
    alpha = 0;
  }
  
  FragColor = vec4(
    color,
    alpha
  );
}
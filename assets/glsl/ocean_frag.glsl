#version 330 core

in VS_OUT {
  vec3 frag_world_pos;
} fs_in;

out vec4 FragColor;

void main() {
  FragColor = vec4(
    // vec3(fs_in.frag_world_pos.y),
    1.0
  );
}
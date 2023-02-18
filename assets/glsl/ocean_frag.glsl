#version 330 core

in VS_OUT {
  vec3 normal;
  vec3 frag_world_pos;
} fs_in;

out vec4 FragColor;

void main() {
  FragColor = vec4(
    fs_in.normal,
    1.0
  );
}
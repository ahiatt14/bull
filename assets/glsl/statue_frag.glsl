#version 330 core

uniform vec3 color = vec3(0,1,0);
uniform vec3 light_dir = vec3(1,0,0);
uniform vec3 light_color = vec3(1,1,1);

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;

out vec4 FragColor;

void main()
{
  float incidence = max(dot(fs_in.normal, -light_dir), 0);

  vec3 diffuse;
  if (incidence > 0.3) {
    diffuse = light_color * 0.4;
  } else if (incidence > 0.25) {
    diffuse = light_color * 0.15;
  } else {
    diffuse = color * incidence;
  }
  
  FragColor = vec4(color + diffuse, 1);
}
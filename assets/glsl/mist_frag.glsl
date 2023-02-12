#version 330 core

uniform sampler2D mist_texture;

uniform vec2 total_uv_scroll;

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;

out vec4 FragColor;

float brightness(vec3 color) {
  return (color.r + color.g + color.b) / 3.0;
}

void main()
{
  vec3 material = texture(
    mist_texture,
    fs_in.tex_uv + total_uv_scroll
  ).rgb;
  
  FragColor = vec4(
    material,
    brightness(material)
  );
}
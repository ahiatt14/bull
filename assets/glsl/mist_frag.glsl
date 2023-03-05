#version 330 core

uniform sampler2D mist_texture;

uniform vec2 uv_scroll;

const vec3 edge_color = vec3(
  255.0 / 255.0,
  255.0 / 255.0,
  30.0 / 255.0
);

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
    fs_in.tex_uv + uv_scroll
  ).rgb;

  float alpha = brightness(material) * brightness(material) * 3 - 1.0;

  // vec3 color = mix(
  //   material,
  //   edge_color,
  //   1.0 - alpha
  // );

  if (brightness(material) < 0.2) discard;
  
  FragColor = vec4(
    material,
    alpha
  );
}
#version 410 core

out vec4 FragColor;

in TES_OUT {
  vec3 normal;
  vec2 tex_uv;
} fs_in;

uniform sampler2D tex;

void main()
{
  // FragColor = texture(
  //   tex,
  //   fs_in.tex_uv
  // );

  FragColor = vec4(
    fs_in.normal,
    1.0
  );
}
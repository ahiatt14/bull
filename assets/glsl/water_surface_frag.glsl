#version 330 core

uniform sampler2D surface_texture;

uniform vec3 fade_color = vec3(0,0,0);

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;

out vec4 FragColor;

void main()
{
  vec3 material =
    texture(surface_texture, fs_in.tex_uv).rgb;
  
  vec3 mixed =
    mix(
      material,
      fade_color,
      gl_FragCoord.z / 1.0 - 0.5
      // TODO: changing the camera's far clip distance
      // will affect this! figure out a param here
    );
  
  FragColor = vec4(
    mixed,
    1
  );
}
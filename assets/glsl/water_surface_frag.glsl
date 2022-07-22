#version 330 core

float calc_perceived_brightness(vec3 t) {
  return (t.r * 0.21 + t.g * 0.72 + t.b * 0.07) / 3.0;
}

uniform sampler2D noise1;

uniform vec3 water_color = vec3(
  65.0/255.0,
  95.0/255.0,
  124.0/255.0
);
// uniform vec3 water_color = vec3(
//   79.0/255.0,
//   101.0/255.0,
//   122.0/255.0
// );
uniform vec3 light_dir = vec3(
  0.14762034939153687,
  -0.0984135662610246,
  0.9841356626102459
);
uniform vec3 light_color = vec3(
  234.0/255.0,
  208.0/255.0,
  146.0/255.0
);

in vec2 TexCoord;
in vec3 normal;
in vec3 frag_world_position;

out vec4 FragColor;

void main()
{
  FragColor = vec4(
    mix(
      light_color,
      water_color * (texture(noise1, TexCoord).rgb * 2),
      max(1 - dot(normal, -light_dir), 0.75)
    ),
    1.0
  );
}
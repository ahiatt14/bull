#version 330 core

float calc_perceived_brightness(vec3 t) {
  return (t.r * 0.21 + t.g * 0.72 + t.b * 0.07) / 3.0;
}

uniform sampler2D cloud_cover;

uniform vec2 shadow_uv = vec2(0, 0);

uniform vec3 color = vec3(
  0.8,
  0.8,
  0.8
);
uniform vec3 light_dir = vec3(
  .707,
  -.707,
  0
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
  // float shadow = calc_perceived_brightness(
  //   texture(cloud_cover, shadow_uv).rgb
  // );

  vec3 shadow = texture(cloud_cover, shadow_uv).rgb;

  vec3 diffuse =
    light_color *
    (1 - max(dot(normal, -light_dir), 0.0));

  FragColor = vec4(
    color * shadow + diffuse,
    1.0
  );
}
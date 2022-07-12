#version 330 core

in vec2 TexCoord;

uniform sampler2D noise1;
uniform sampler2D noise2;

uniform vec3 hot_color = vec3(1.0);
uniform vec3 cool_color = vec3(0);

float calc_perceived_brightness(vec3 t) {
  return (t.r * 0.21 + t.g * 0.72 + t.b * 0.07) / 3.0;
}

float calc_brightness(vec3 t) {
  return (t.r + t.g + t.b) / 3.0;
}

out vec4 FragColor;
void main()
{
  // FragColor = texture(noise1, TexCoord);

  // float temp = length(texture(noise1, TexCoord)) / 4.0;
  float temp = calc_perceived_brightness(
    texture(noise1, TexCoord).rgb - 0.2
  );
  float brightness = calc_perceived_brightness(
    texture(noise2, TexCoord).rgb + 0.2
  );
  vec3 final = mix(hot_color, cool_color, temp * 3) + brightness;

  FragColor = vec4(final, 1.0);
}
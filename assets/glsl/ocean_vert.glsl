#version 330 core

#define PI 3.1415926536

layout (location = 0) in vec3 position;

uniform mat4 model = mat4(
  vec4(1.0, 0.0, 0.0, 0.0),
  vec4(0.0, 1.0, 0.0, 0.0),
  vec4(0.0, 0.0, 1.0, 0.0),
  vec4(0.0, 0.0, 0.0, 1.0)
);
uniform mat4 view = mat4(
  vec4(1.0, 0.0, 0.0, 0.0),
  vec4(0.0, 1.0, 0.0, 0.0),
  vec4(0.0, 0.0, 1.0, 0.0),
  vec4(0.0, 0.0, 0.0, 1.0)
);
uniform mat4 projection = mat4(
  vec4(1.0, 0.0, 0.0, 0.0),
  vec4(0.0, 1.0, 0.0, 0.0),
  vec4(0.0, 0.0, 1.0, 0.0),
  vec4(0.0, 0.0, 0.0, 1.0)
);

uniform float w = 2.5; // wavelength
uniform float a = 1; // amplitude

out VS_OUT {
  vec3 normal;
  vec3 frag_world_pos;
} vs_out;

void main() {

  float k = 2 * PI / w;

  float y = a * sin(k * position.x);

  vec3 wave_pos = position;
  wave_pos.y = y;

  vs_out.frag_world_pos = vec3(model * vec4(wave_pos, 1.0));

  gl_Position = projection * view * model * vec4(wave_pos, 1.0);
}
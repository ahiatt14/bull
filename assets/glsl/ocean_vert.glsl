#version 330 core

#define PI 3.1415926536

layout (location = 0) in vec3 position;
// layout (location = 1) in vec3 local_normal;
// layout (location = 2) in vec2 uv;

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

uniform float total_elapsed_seconds;

uniform float s = 1; // speed
uniform float w = 1; // wavelength
uniform float a = 0.1; // amplitude

out VS_OUT {
  vec3 normal;
  vec3 frag_world_pos;
} vs_out;

void main() {

  float k = 2 * PI / w;

  float y = a * sin(k * position.x - s * total_elapsed_seconds);

  vec3 wave_pos = position;
  wave_pos.y = y;

  vs_out.frag_world_pos = vec3(model * vec4(wave_pos, 1.0));

  gl_Position = projection * view * model * vec4(wave_pos, 1.0);
}
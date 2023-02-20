#version 330 core

#define PI 3.1415926536
#define GRAVITY 0.5

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
uniform mat3 normals_model = mat3(
  vec3(1.0, 0.0, 0.0),
  vec3(0.0, 1.0, 0.0),
  vec3(0.0, 0.0, 1.0)
);

uniform float total_elapsed_seconds;

uniform vec2 direction = vec2(0, -1);

uniform float wavelength = 1;
uniform float steepness = 1;

out VS_OUT {
  vec3 normal;
  vec3 frag_world_pos;
} vs_out;

void main() {

  vec3 p = position;

  float k = 2.0 * PI / wavelength;
  float phase_speed = sqrt(GRAVITY / k);
  float f =
    k * (
      dot(direction.xy, p.xz) -
      phase_speed * total_elapsed_seconds
    );
  float a = steepness / k;
  
  p.x += direction.x * a * cos(f);
  p.y = a * sin(f);
  p.z += direction.y * a * cos(f);

  vec3 tangent = vec3(
    1.0 - direction.x * direction.x * steepness * sin(f),
    direction.x * steepness * cos(f),
    -direction.x * direction.y * steepness * cos(f)
  );
  vec3 binormal = vec3(
    -direction.x * direction.y * steepness * sin(f),
    direction.y * steepness * cos(f),
    1.0 - direction.y * direction.y * steepness * sin(f)
  );
  vec3 normal = normalize(cross(binormal, tangent));

  vs_out.normal = normalize(normals_model * normal);
  vs_out.frag_world_pos = vec3(model * vec4(p, 1.0));

  gl_Position = projection * view * model * vec4(p, 1.0);
}
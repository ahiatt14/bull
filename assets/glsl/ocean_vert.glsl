#version 330 core

#define PI 3.1415926536
#define GRAVITY 0.4

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 local_normal;
layout (location = 2) in vec2 uv;

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

uniform vec3 cam_world_pos = vec3(0);

out VS_OUT {
  vec3 normal;
  vec3 frag_world_pos;
  vec2 tex_uv;
} vs_out;

vec3 tangent;
vec3 binormal;
vec3 gerstner_wave(
  vec2 direction,
  float wavelength,
  float steepness,
  vec3 pos
) {
  float k = 2.0 * PI / wavelength;
  float phase_speed = sqrt(GRAVITY / k);
  float f =
    k * (
      dot(direction.xy, pos.xz) -
      phase_speed * total_elapsed_seconds
    );
  float a = steepness / k;

  tangent += vec3(
    1.0 - direction.x * direction.x * steepness * sin(f),
    direction.x * steepness * cos(f),
    -direction.x * direction.y * steepness * cos(f)
  );
  binormal += vec3(
    -direction.x * direction.y * steepness * sin(f),
    direction.y * steepness * cos(f),
    1.0 - direction.y * direction.y * steepness * sin(f)
  );
  return vec3(
    direction.x * a * cos(f),
    a * sin(f),
    direction.y * a * cos(f)
  );
}

void main() {

  vs_out.tex_uv = uv;

  vec3 pos = position;
  // pos.x = pos.x + cam_world_pos.x;
  // pos.z = pos.z + cam_world_pos.z;

  binormal = vec3(0);
  tangent = vec3(0);

  pos += gerstner_wave(normalize(vec2(0.2, -1)), 6, 0.2, pos);
  pos += gerstner_wave(normalize(vec2(-0.6, -1)), 5, 0.15, pos);
  pos += gerstner_wave(normalize(vec2(1, 1)), 3, 0.15, pos);

  vec4 vert_world_pos = model * vec4(pos, 1.0);

  vert_world_pos.y =
    vert_world_pos.y /
    max(
      .5 * sqrt(length(position)),
      1.0
    );

  vec3 normal = normalize(cross(binormal, tangent));
  vs_out.normal = normalize(normals_model * normal);
  vs_out.frag_world_pos = vert_world_pos.xyz;

  gl_Position = projection * view * vert_world_pos;
}
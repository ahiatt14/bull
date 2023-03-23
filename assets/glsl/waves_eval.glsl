#version 410 core

layout (quads, equal_spacing, ccw) in;

const float PI = 3.1415926536;

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

uniform float gravity = 9.8;

uniform vec2 wave0_dir = vec2(1, 0);
uniform vec2 wave0_props = vec2(50, 0.5);
uniform vec2 wave1_dir = vec2(1, -1);
uniform vec2 wave1_props = vec2(40, 0.3);
uniform vec2 wave2_dir = vec2(0, -1);
uniform vec2 wave2_props = vec2(30, 0.2);

in vec2 texture_uv[];

out TES_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} tes_out;

vec3 binormal;
vec3 tangent;
vec3 gerstner_wave(
  vec2 direction,
  float wavelength,
  float steepness,
  vec3 pos
) {
  float k = 2.0 * PI / wavelength;
  float phase_speed = sqrt(gravity / k);
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

  float u = gl_TessCoord.x;
  float v = gl_TessCoord.y;

  vec2 uv0 = texture_uv[0];
  vec2 uv1 = texture_uv[1];
  vec2 uv2 = texture_uv[2];
  vec2 uv3 = texture_uv[3];

  vec2 left_uv = uv0 + v * (uv3 - uv0);
  vec2 right_uv = uv1 + v * (uv2 - uv1);
  tes_out.tex_uv = left_uv + u * (right_uv - left_uv);

  vec3 p0 = gl_in[0].gl_Position.xyz;
  vec3 p1 = gl_in[1].gl_Position.xyz;
  vec3 p2 = gl_in[2].gl_Position.xyz;
  vec3 p3 = gl_in[3].gl_Position.xyz;

  vec3 left_pos = p0 + v * (p3 - p0);
  vec3 right_pos = p1 + v * (p2 - p1);
  vec3 pos = left_pos + u * (right_pos - left_pos);

  binormal = vec3(0);
  tangent = vec3(0);

  pos += gerstner_wave(wave0_dir, wave0_props.x, wave0_props.y, pos);
  pos += gerstner_wave(wave1_dir, wave1_props.x, wave1_props.y, pos);
  pos += gerstner_wave(wave2_dir, wave2_props.x, wave2_props.y, pos);

  vec3 normal = normalize(cross(binormal, tangent));
  tes_out.normal = normals_model * normal;

  tes_out.world_frag_pos = vec3(model * vec4(pos, 1.0));

  gl_Position = projection * view * model * vec4(pos, 1.0);
}
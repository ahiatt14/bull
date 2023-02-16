#version 330 core

out vec4 FragColor;

const vec3 start_color = vec3(0.918, 0.816, 0.573);
// const vec3 end_color = vec3(1, 0.776, 0);

// uniform float seconds_since_activation;
// uniform float limit_in_seconds;

void main()
{

  FragColor = vec4(start_color, 1.0);

  // FragColor = vec4(
  //   mix(
  //     start_color,
  //     end_color,
  //     seconds_since_activation / limit_in_seconds
  //   ),
  //   1.0
  // );
}
R"(
#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 projectionMatrix = mat4(1.0);
uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 transformationMatrix = mat4(1.0);

void main()
{
  gl_Position = projectionMatrix * viewMatrix * transformationMatrix * vec4(position, 1.0);
}
)"

R"skyboxshader(
//vertex
#version 330 core

layout(location = 0) in vec3 position;
out vec3 vert_texCoord;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{
  vec3 worldPos = mat3(viewMatrix) * position;
	gl_Position = projectionMatrix * vec4(worldPos, 1.0);
	vert_texCoord = position;
}

//fragment
#version 330 core

in vec3 vert_texCoord;
out vec4 out_color;
uniform samplerCube textureSampler;

void main()
{
	out_color = texture(textureSampler,vert_texCoord);
}
)skyboxshader"

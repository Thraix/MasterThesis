//vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 3) in vec3 normal;

out vec4 vert_color;
out vec3 surfaceNormal;
out vec3 toLightVector;
out vec3 toCameraVector;

uniform vec3 u_Color;
uniform mat4 u_ModelMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
uniform vec3 u_SunDir = vec3(0.0, 10.0, -10.0);

void main()
{
	vec4 worldPosition = u_ModelMatrix * vec4(position, 1.0f);
	vec4 positionRelativeToCamera = u_ViewMatrix * worldPosition;
	gl_Position = u_ProjectionMatrix * positionRelativeToCamera;
  vert_color = vec4(u_Color,1.0);

	surfaceNormal = mat3(u_ModelMatrix) * normal;
	toLightVector = u_SunDir;
	toCameraVector = (inverse(u_ViewMatrix) * vec4(0, 0, 0, 1)).xyz - worldPosition.xyz;
}

//fragment
#version 330 core

in vec4 vert_color;
in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 toCameraVector;

layout(location = 0) out vec4 out_color;

uniform vec3 light_color = vec3(1.0f, 1.0f, 1.0f);
uniform float specularExponent = 10.0;
uniform float specularStrength = 1;
uniform float diffuseStrength = 1;
uniform float ambient = 0.3;

void main()
{
	out_color = vert_color;
	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitLightVector = normalize(toLightVector);
	vec3 unitVectorToCamera = normalize(toCameraVector);
	vec3 lightDirection = -unitLightVector;

	float diffuse = diffuseStrength * max(dot(unitNormal, unitLightVector), 0.0);
	float specular = specularStrength * pow(max(dot(reflect(lightDirection, unitNormal), unitVectorToCamera), 0.0f), specularExponent);

	out_color *= vec4((ambient + (diffuse + specular) * light_color), 1.0f);
}

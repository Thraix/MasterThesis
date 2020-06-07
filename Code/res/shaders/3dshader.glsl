//vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 3) in vec3 normal;

out vec4 worldPos;
out vec4 color;
out vec3 toLightVector;
out vec3 toCameraVector;
out vec3 surfaceNormal;

uniform float u_Size = 128;
uniform vec3 u_SunDir;
uniform vec3 u_Color;
uniform mat4 u_ModelMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;

void main()
{
	worldPos = u_ModelMatrix * vec4(position, 1.0f);
  /* worldPos = vec4(vec3(floor(worldPos)), worldPos.w); */
  /* worldPos = vec4(min(worldPos.xyz, u_Size-1) + 0.5, worldPos.w); */
	vec4 positionRelativeToCamera = u_ViewMatrix * worldPos;
	gl_Position  = u_ProjectionMatrix * positionRelativeToCamera;
  color = vec4(u_Color,1.0);
	toCameraVector = (inverse(u_ViewMatrix) * vec4(0, 0, 0, 1)).xyz - worldPos.xyz;
  toLightVector = u_SunDir;
  surfaceNormal = normal;
}

//fragment
#version 330 core

in vec4 worldPos;
in vec4 color;
in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 toCameraVector;

layout(location = 0) out vec4 out_color;

uniform float u_Size = 128;
uniform vec3 light_color = vec3(1.0f, 1.0f, 1.0f);
uniform float specularExponent = 10.0;
uniform float specularStrength = 1;
uniform float diffuseStrength = 1;
uniform float ambient = 0.3;

void main()
{
  if(worldPos.x < 0.0f || worldPos.y < 0.0f || worldPos.z < 0.0f ||
     worldPos.x > u_Size || worldPos.y > u_Size || worldPos.z > u_Size)
    discard;
	out_color = color;
	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitLightVector = normalize(toLightVector);
	vec3 unitVectorToCamera = normalize(toCameraVector);
	vec3 lightDirection = -unitLightVector;

	float diffuse = diffuseStrength * max(dot(unitNormal, unitLightVector), 0.0);
	float specular = specularStrength * pow(max(dot(reflect(lightDirection, unitNormal), unitVectorToCamera), 0.0f), specularExponent);

	out_color *= vec4((ambient + (diffuse + specular) * light_color), 1.0f);
}

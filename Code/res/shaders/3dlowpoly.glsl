//vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 3) in vec3 normal;

out vert_out {
  vec4 pos;
  vec4 worldPos;
  vec4 color;
  vec3 toCameraVector;
} v_out;

uniform vec3 u_Color;
uniform mat4 u_ModelMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;

void main()
{
	v_out.worldPos = u_ModelMatrix * vec4(position, 1.0f);
  worldPosition = vec4(vec3(floor(worldPosition) + 0.5), worldPosition.w);
	vec4 positionRelativeToCamera = u_ViewMatrix * v_out.worldPos;
	v_out.pos = u_ProjectionMatrix * positionRelativeToCamera;
  v_out.color = vec4(u_Color,1.0);
	v_out.toCameraVector = (inverse(u_ViewMatrix) * vec4(0, 0, 0, 1)).xyz - v_out.worldPos.xyz;
}

//geometry
#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vert_out {
    vec4 pos;
    vec4 worldPos;
    vec4 color;
    vec3 toCameraVector;
} gs_in[];

out vec4 worldPos;
out vec3 surfaceNormal;
out vec3 toLightVector;
out vec3 toCameraVector;
out vec4 color;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
uniform vec3 u_SunDir = vec3(0.0, 10.0, -10.0);

void main()
{
  vec3 normal = cross(vec3(gs_in[1].worldPos - gs_in[0].worldPos),vec3(gs_in[2].worldPos - gs_in[0].worldPos));

  for(int i = 0;i<3;i++)
  {
    gl_Position = gs_in[i].pos;
    worldPos = gs_in[i].worldPos;
    surfaceNormal = normal;
    toLightVector = u_SunDir;
    toCameraVector = gs_in[i].toCameraVector;
    color = gs_in[i].color;
    EmitVertex();
  }
  EndPrimitive();
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

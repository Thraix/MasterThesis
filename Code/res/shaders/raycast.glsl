//fragment
#version 450 core

in vec3 v_Near;
in vec3 v_Dir;
in vec3 v_CameraPos;

out vec4 f_Color;

uniform sampler3D u_ChunkTexUnit;

uniform float u_MaxRayLength = 1000;
uniform int u_Size;
uniform vec3 u_SunDir;

struct Ray
{
  vec3 pos;
  vec3 dir;
  float rayLength;
};

struct RayIntersection
{
  float voxelValue;
  vec3 collisionPoint;
  float rayLength;
  vec3 normal;
  bool found;
  vec3 voxelPos;
};

float ambient = 0.3;

int intersectionAxis[3][3] = {{0,2,1}, {1,0,2}, {2,0,1}};

bool HasVoxel(float value)
{
  return value > 0.0f;
}

float GetVoxel(vec3 coord)
{
  if(coord.x < 0 || coord.y < 0 || coord.z < 0 || coord.x >= u_Size || coord.y >= u_Size || coord.z >= u_Size)
    return 0;
  return texture(u_ChunkTexUnit, coord / u_Size).r * 255;
}

vec3 RayColor(Ray ray, RayIntersection intersection, vec3 color, float brightness)
{
  brightness *= (intersection.voxelPos.x + intersection.voxelPos.y + intersection.voxelPos.z) / (1.5 * u_Size);
  if(intersection.voxelValue == 1.0)
    return vec3(0.2, 0.8, 0.2) * brightness;
  else if(round(intersection.voxelValue) == 2.0)
    return vec3(0.8, 0.2, 0.2) * brightness;
  else if(round(intersection.voxelValue) == 3.0)
    return vec3(0.8, 0.8, 0.2) * brightness;
  else if(round(intersection.voxelValue) == 4.0)
    return vec3(0.2, 0.2, 0.8) * brightness;
  else if(round(intersection.voxelValue) == 5.0)
    return vec3(0.8, 0.2, 0.8) * brightness;
  else
    return vec3(1) * brightness;
}

Ray GetShadowRay(Ray ray, RayIntersection intersection)
{
  Ray shadowRay;
  shadowRay.pos = intersection.collisionPoint;
  shadowRay.dir = normalize(u_SunDir);
  shadowRay.rayLength = intersection.rayLength;
  return shadowRay;
}

bool TestCube(vec3 currentPos, vec3 dir, vec3 centerPos, vec3 size)
{
  return !
    ((currentPos.x > centerPos.x + size.x / 2 && dir.x > 0) ||
     (currentPos.x < centerPos.x - size.x / 2 && dir.x < 0) ||
     (currentPos.y > centerPos.y + size.y / 2 && dir.y > 0) ||
     (currentPos.y < centerPos.y - size.y / 2 && dir.y < 0) ||
     (currentPos.z > centerPos.z + size.z / 2 && dir.z > 0) ||
     (currentPos.z < centerPos.z - size.z / 2 && dir.z < 0));
}

bool RayMarchShadow(inout Ray ray)
{
  float rayLength = ray.rayLength;
  vec3 currentPos = ray.pos;
  vec3 currentVoxel = floor(ray.pos);
  vec3 nextPlane = vec3(
      ray.dir.x < 0 ? ceil(ray.pos.x-1) : floor(ray.pos.x+1),
      ray.dir.y < 0 ? ceil(ray.pos.y-1) : floor(ray.pos.y+1),
      ray.dir.z < 0 ? ceil(ray.pos.z-1) : floor(ray.pos.z+1));

  vec3 stepDir = sign(ray.dir);

  vec3 t = (nextPlane - ray.pos) / ray.dir;

  while(rayLength < u_MaxRayLength)
  {
    if(!TestCube(currentPos, ray.dir, vec3(u_Size*0.5), vec3(u_Size)))
    {
      return false;
    }
    float tMin = min(t.x, min(t.y, t.z));
    t -= tMin;
    rayLength += tMin;

    int axis = 0;
    if(t.x == 0) axis = 0;
    else if(t.y == 0) axis = 1;
    else if(t.z == 0) axis = 2;

    currentVoxel[axis] += stepDir[axis];

    if(HasVoxel(GetVoxel(currentVoxel)))
    {
      return true;
    }
    t[axis] = 1.0f / abs(ray.dir[axis]);
  }
  return false;
}

RayIntersection RayMarch(inout Ray ray)
{
  float rayLength = ray.rayLength;
  vec3 currentPos = ray.pos;
  vec3 currentVoxel = floor(ray.pos);
  vec3 nextPlane = vec3(
      ray.dir.x < 0 ? ceil(currentPos.x-1) : floor(currentPos.x+1),
      ray.dir.y < 0 ? ceil(currentPos.y-1) : floor(currentPos.y+1),
      ray.dir.z < 0 ? ceil(currentPos.z-1) : floor(currentPos.z+1));

  vec3 stepDir = sign(ray.dir);

  vec3 t = (nextPlane - ray.pos) / ray.dir;
  int internalReflection = 0;

  while(rayLength < u_MaxRayLength)
  {
    if(!TestCube(currentPos, ray.dir, vec3(u_Size*0.5), vec3(u_Size)))
    {
      return RayIntersection(0, vec3(0,0,0), 0, vec3(0), false, vec3(0,0,0));
    }
    float tMin = min(t.x, min(t.y, t.z));
    t -= tMin;
    rayLength += tMin;
    vec3 oldPos = currentPos;
    currentPos = ray.pos + (rayLength - ray.rayLength) * ray.dir;

    int axis = 0;
    if(t.x == 0) axis = 0;
    else if(t.y == 0) axis = 1;
    else if(t.z == 0) axis = 2;

    currentVoxel[axis] += stepDir[axis];

    vec3 normal = vec3(0);
    normal[axis] = -stepDir[axis];

    float voxel = GetVoxel(currentVoxel);
    if(HasVoxel(voxel))
    {
      RayIntersection intersection = RayIntersection(
          voxel,
          currentPos,
          rayLength,
          normal,
          true,
          currentVoxel);
      return intersection;
    }
    t[axis] = 1.0f / abs(ray.dir[axis]);
  }
  return RayIntersection(0.0, vec3(0,0,0), 0, vec3(0), false, vec3(0,0,0));
}

vec3 GetSkyboxColor(Ray ray, vec3 color)
{
  return vec3(0.2, 0.2, 0.8);
  vec3 unitDir = normalize(ray.dir);
  float sun = 10 * pow(dot(normalize(u_SunDir), unitDir), 400.0);
  float grad = (unitDir.y + 1.0) * 0.5;
  vec3 skyboxColor = max(vec3(0,grad*0.75,grad), vec3(sun, sun, 0)) * max(u_SunDir.y, 0.0);
  return skyboxColor;
}

RayIntersection TraceWithShadow(Ray ray, inout vec3 color)
{
  RayIntersection intersection = RayMarch(ray);
  if(intersection.found)
  {
    // Shadow ray
    bool inShadow = false;
    /* Ray shadowRay = GetShadowRay(ray, intersection); */
    /* bool inShadow = RayMarchShadow(shadowRay); */
    float brightness = 0.0f;
    if(inShadow)
    {
      // Full shadow
      brightness = ambient;
    }
    else
    {
      float diffuse = (1.0 - ambient) * abs(dot(intersection.normal, normalize(u_SunDir)));
      float specular = 0.0 * pow(abs(dot(reflect(normalize(u_SunDir), intersection.normal), ray.dir)), 60);
      brightness  = ambient + diffuse + specular;
    }
    color = RayColor(ray, intersection, color, brightness);
  }
  return intersection;
}

void main()
{
  vec3 color = vec3(0,0,0);
  Ray ray = Ray(v_Near, normalize(v_Dir), 0);
  RayIntersection intersection = TraceWithShadow(ray, color);
  if(!intersection.found)
    discard;
  f_Color = vec4(color, 1.0);
}

//vertex
#version 450 core

layout(location = 0) in vec2 a_Position;

out vec3 v_Near;
out vec3 v_Dir;
out vec3 v_CameraPos;

uniform mat4 u_ViewMatrix;
uniform mat4 u_PVInvMatrix;
uniform vec3 cameraPos;

void main()
{
  vec4 near4 = u_PVInvMatrix * vec4(a_Position, -1.0f, 1.0);
  vec4 far4 = u_PVInvMatrix * vec4(a_Position, 1.0f, 1.0);
  v_Near = vec3(near4) / near4.w;
  v_Dir = vec3(far4) / far4.w - v_Near;
  gl_Position = vec4(a_Position, 0.0f, 1.0f);
  v_CameraPos = vec3(inverse(u_ViewMatrix) * vec4(0,0,0,1));
}

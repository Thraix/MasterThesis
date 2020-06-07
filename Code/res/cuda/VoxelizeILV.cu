#pragma once

#include "Common.cu"

#include "VectorMath.cu"

struct LineData
{
  int3 t;
  int3 tStep;
  int3 signDir;
  int3 pos;
  int3 end;
};

__device__
int3& GetNextVoxelInLine(LineData& data)
{
  int tmin = min(data.t);
  int axis = 0;

  if(tmin == data.t.x && data.signDir.x != 0) axis = 0;
  else if(tmin == data.t.y && data.signDir.y != 0) axis = 1;
  else if(tmin == data.t.z && data.signDir.z != 0) axis = 2;

  data.t -= tmin;

  get(data.pos, axis) += get(data.signDir, axis);
  get(data.t, axis) = get(data.tStep, axis);
  return data.pos;
}

__device__
LineData CreateLineData(int3 v1, int3 v2)
{
  LineData data;
  int3 dir = v2 - v1;
  data.signDir = sign(dir);
  data.pos = v1;
  data.end = v2;

  int3 dir1 = make_int3(max(abs(dir.x), 1),max(abs(dir.y), 1),max(abs(dir.z), 1));

  data.t = make_int3(dir1.y * dir1.z, dir1.x * dir1.z, dir1.x * dir1.y);
  if(dir.x == 0) data.t.x = INT_MAX;
  if(dir.y == 0) data.t.y = INT_MAX;
  if(dir.z == 0) data.t.z = INT_MAX;
  data.tStep = data.t * 2;
  return data;
}

__device__
void voxelizeLine(int3 v1, int3 v2, unsigned char color)
{
  LineData data = CreateLineData(v1,v2);
  voxelizePoint(data.pos, color);

  while(data.pos != data.end)
  {
    voxelizePoint(GetNextVoxelInLine(data), color);
  }
}

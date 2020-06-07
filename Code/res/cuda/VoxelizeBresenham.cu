#pragma once

#include "Common.cu"

#include "VectorMath.cu"

struct LineData
{
  int3 pos;
  int3 end;
  int3 signDir;
  int axis;
  int axisX;
  int axisY;

  int eX;
  int eY;
  int state;

  int deX;
  int deY;
  int deAxis;
  int3 nextPos;
};

__device__
int3 GetNextVoxelInLine(LineData& data)
{
  // This state based system is bad for branching prediction...
  // But I don't know how to implement getting the next voxel otherwise.
  // And the scanline algorithm requires it in the interior voxelization.

  int3 lastVoxel = data.pos;
  data.pos = data.nextPos;

  // State0
  if(data.state == 0)
  {
    data.state = 1;
    get(data.nextPos, data.axis) += get(data.signDir, data.axis);
    if(data.pos != lastVoxel)
      return data.pos;
    data.pos = data.nextPos;
  }

  // State1
  if(data.state == 1)
  {
    data.state = 2;
    int lastEX = data.eX;
    data.eX += data.deX;
    if(lastEX >= 0)
    {
      data.eX -= data.deAxis;
      get(data.nextPos, data.axisX) += get(data.signDir, data.axisX);
      if(2*lastEX > data.deX)
      {
        data.pos = lastVoxel;
        get(data.pos, data.axisX) += get(data.signDir, data.axisX);
      }
      if(data.pos != lastVoxel)
        return data.pos;
      data.pos = data.nextPos;
    }
  }
  // State2
  if(data.state == 2)
  {
    data.state = 0;
    int lastEY = data.eY;
    data.eY += data.deY;
    if(lastEY >= 0)
    {
      data.eY -= data.deAxis;
      get(data.nextPos, data.axisY) += get(data.signDir, data.axisY);
      if(2*lastEY > data.deY)
      {
        data.pos = lastVoxel;
        get(data.pos, data.axisY) += get(data.signDir, data.axisY);
      }
      if(data.pos != lastVoxel)
        return data.pos;
      data.pos = data.nextPos;
    }
  }

  // State0
  data.state = 1;
  get(data.nextPos, data.axis) += get(data.signDir, data.axis);
  return data.pos;
}

__device__
LineData CreateLineData(int3 v1, int3 v2)
{
  LineData data;
  int3 diff = v2 - v1;
  data.pos = v1;
  data.nextPos = data.pos;
  data.end = v2;
  data.signDir = sign(diff);
  diff = abs(diff);
  int maxDistance = max(diff);
  if(maxDistance == diff.x) data.axis = 0;
  else if(maxDistance == diff.y) data.axis = 1;
  else if(maxDistance == diff.z) data.axis = 2;
  data.axisX = (data.axis+1) % 3;
  data.axisY = (data.axis+2) % 3;
  if(data.axisX > data.axisY)
  {
    int tmp = data.axisX;
    data.axisX = data.axisY;
    data.axisY = tmp;
  }
  data.eX = 2 * get(diff, data.axisX) - get(diff, data.axis);
  data.eY = 2 * get(diff, data.axisY) - get(diff, data.axis);
  data.state = 0;

  data.deX = 2 * get(diff, data.axisX);
  data.deY = 2 * get(diff, data.axisY);
  data.deAxis = 2 * get(diff, data.axis);

  return data;
}

__device__
void voxelizeLine(int3 v1, int3 v2, unsigned char color)
{
  LineData data = CreateLineData(v1,v2);
  int3 lastVoxel = data.pos;

  while(data.pos != data.end)
  {
    lastVoxel = data.pos;
    voxelizePoint(data.pos, color);
    get(data.pos, data.axis) += get(data.signDir, data.axis);

    int lastEX = data.eX;
    data.eX += data.deX;
    if(lastEX >= 0)
    {
      int diff = get(data.signDir, data.axisX);

      if(2*lastEX <= data.deX)
        lastVoxel = data.pos;
      else
        get(lastVoxel, data.axisX) += diff;

      voxelizePoint(lastVoxel, color);
      get(data.pos, data.axisX) += diff;
      data.eX -= data.deAxis;
    }

    int lastEY = data.eY;
    data.eY += data.deY;
    if(lastEY >= 0)
    {
      int diff = get(data.signDir, data.axisY);

      if(2*lastEY <= data.deY)
        lastVoxel = data.pos;
      else
        get(lastVoxel, data.axisY) += diff;

      voxelizePoint(lastVoxel, color);
      get(data.pos, data.axisY) += diff;
      data.eY -= data.deAxis;
    }
  }
  voxelizePoint(data.pos, color);
}

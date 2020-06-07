#pragma once

#include "Common.cu"

#include "VectorMath.cu"

#if __YCM__
#define _BRESENHAM
#endif

namespace ILV
{

#if defined(_ILV)
#include "VoxelizeILV.cu"
#elif defined(_BRESENHAM)
#include "VoxelizeBresenham.cu"
#else
#error No Algorithm specified
#endif

  __device__
  int3 getNextScanline(LineData& data, int3 pos, int2 diff, int axis, int axisX, int axisY, bool increaseScanline)
  {
    int z = get(pos, axis);
    int3 nextPos = data.pos;

    int2 increment = make_int2(diff.y, -diff.x);
    int c = get(pos, axisX) * increment.x + get(pos, axisY) * increment.y;
    int cLower = c - abs(diff.x) - abs(diff.y);
    int cUpper = c + abs(diff.x) + abs(diff.y);
    int cEnd = get(data.end, axisX) * increment.x + get(data.end, axisY) * increment.y;
    int cNext = get(nextPos, axisX) * increment.x + get(nextPos, axisY) * increment.y;

    // This resolves holes in the voxelization in some edge cases when
    // scanline doesn't fill all the voxels. This happens when a new slice
    // is started and the scanline goes backwards.
    // This then looks for that problem and limits the scanline to only
    // be behind the old one
    if((cEnd - c) * (cNext - c) < 0)
    {
      if(cEnd > c)
        cUpper = c;
      else
        cLower = c;
    }
    else if(!increaseScanline)
    {
      return pos;
    }

    while(get(nextPos, axis) == z && pos != data.end && cLower <= c && c <= cUpper)
    {
      pos = nextPos;
      if(data.pos != data.end)
        nextPos = GetNextVoxelInLine(data);
      c = get(nextPos, axisX) * increment.x + get(nextPos, axisY) * increment.y;
    }
    return pos;
  }

  // Check if the 'inside' line is between 'vec1' and 'vec2'.
  // Source: https://stackoverflow.com/a/17497339
  __device__
  bool VectorInside(int2 vec1, int2 vec2, int2 inside)
  {
    return
      ((vec1.y * inside.x - vec1.x * inside.y) * (vec1.y * vec2.x - vec1.x * vec2.y) >= 0 && (vec2.y * inside.x - vec2.x * inside.y) * (vec2.y * vec1.x - vec2.x * vec1.y) >= 0) ||
      ((-vec1.y * inside.x + vec1.x * inside.y) * (vec1.y * vec2.x - vec1.x * vec2.y) >= 0 && (-vec2.y * inside.x + vec2.x * inside.y) * (vec2.y * vec1.x - vec2.x * vec1.y) >= 0);
  }

  __device__
  void voxelizeInterior(int3 v1, int3 v2, int3 v3, int axis)
  {
    int axisX = (axis + 1) % 3;
    int axisY = (axis + 2) % 3;

    int2 scanlineDir = toInt2(cross(v2 - v1, v3 - v1), axisY, axisX);
    scanlineDir.x = -scanlineDir.x;

    bool swapped = false;
    LineData dataFrom = CreateLineData(v1, v2);
    LineData dataTo = CreateLineData(v1, v3);
    LineData dataEdge = CreateLineData(v2, v3);

    int3 posFrom = dataFrom.pos;
    int3 posTo = dataTo.pos;

    int2 diff = toInt2(posTo - posFrom, axisX, axisY);

    int2 d13 = toInt2(v3 - v1, axisX, axisY);
    int2 d12 = toInt2(v2 - v1, axisX, axisY);

    int increaseScanline = true;
    if(get(v1, axis) != get(v3, axis))
      diff = scanlineDir;
    else
    {
      swapped = true;
      dataFrom = dataEdge;
      posFrom = dataFrom.pos;
      diff = toInt2(posTo - posFrom, axisX, axisY);
      increaseScanline = false;
    }

    while(posTo != v3 || posFrom != v3)
    {
      posFrom = getNextScanline(dataFrom, posFrom, diff, axis, axisX, axisY, increaseScanline);
      posTo = getNextScanline(dataTo, posTo, diff, axis, axisX, axisY, increaseScanline);
      increaseScanline = true;

      voxelizeLine(posFrom, posTo, 1);

      if(posFrom == v2 && !swapped)
      {
        dataFrom = dataEdge;
        swapped = true;
      }

      diff = toInt2(posTo - posFrom, axisX, axisY);
      if(get(posFrom, axis) != get(dataFrom.pos, axis) && get(posTo, axis) != get(dataTo.pos, axis))
      {
        posFrom = dataFrom.pos;
        posTo = dataTo.pos;

        if(dataFrom.pos != dataFrom.end)
          GetNextVoxelInLine(dataFrom);
        if(dataTo.pos != dataTo.end)
          GetNextVoxelInLine(dataTo);

        increaseScanline = false;
      }
      if(!swapped && VectorInside(d13, d12, diff))
      {
        diff = scanlineDir;
      }
    }
  }

  __device__
  void voxelize(float3 v1, float3 v2, float3 v3)
  {
    int3 vi1 = toInt3(v1);
    int3 vi2 = toInt3(v2);
    int3 vi3 = toInt3(v3);

    // Calculate most dominant axis
    int3 normal = abs(cross(vi2 - vi1, vi3 - vi1));
    int domNormal = max(normal);
    int axis = 0;
    if(domNormal == normal.x) axis = 0;
    else if(domNormal == normal.y) axis = 1;
    else if(domNormal == normal.z) axis = 2;

    // Sort by most dominant axis
    if(get(vi1, axis) > get(vi2, axis))
      swap(vi1,vi2);
    if(get(vi1, axis) > get(vi3, axis))
      swap(vi1,vi3);
    if(get(vi2, axis) > get(vi3, axis))
      swap(vi2,vi3);

    // Voxelize edges
    voxelizeLine(vi1, vi2, 1);
    voxelizeLine(vi2, vi3, 1);
    voxelizeLine(vi1, vi3, 1);

    // Voxelize interior
    voxelizeInterior(vi1, vi2, vi3, axis);
  }
}

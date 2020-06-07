#pragma once

#include "Common.cu"

#include "VectorMath.cu"

namespace RLV
{
  struct ScanlineCache
  {
    float3 v1;
    float3 v2;
    float3 v3;

    float3 unitDir12;
    float3 unitDir13;
    float3 unitDir23;


    float2 projUnitDir12;
    float2 projUnitDir13;
    float2 projUnitDir23;

    float lengthDir12;
    float lengthDir13;
    float minLengthDir23;
    float lengthDir23;

    float scanlineLength;
    float scanlineInvDirAxis;
    float scanlineMaxLength;

    float slInvDotDir12; //  1 / dot(projSlDir, projUnitDir12)
    float slInvDotDir13; //  1 / dot(projSlDir, projUnitDir13)
    float slInvDotDir23; //  1 / dot(projSlDir, projUnitDir23)
  };

  __device__
  void voxelizeLine(float3 v1, float3 v2, unsigned char color)
  {
    float3 dir = v2 - v1;
    float dirLength = length(dir);
    float3 unitDir = normalize(dir);
    float3 signDir = sign(dir);

    float3 nextPlane = make_float3(
        dir.x < 0 ? ceil(v1.x - 1) : floor(v1.x + 1),
        dir.y < 0 ? ceil(v1.y - 1) : floor(v1.y + 1),
        dir.z < 0 ? ceil(v1.z - 1) : floor(v1.z + 1)
        );

    int3 pos = make_int3(
        dir.x < 0 ? nextPlane.x : nextPlane.x-1,
        dir.y < 0 ? nextPlane.y : nextPlane.y-1,
        dir.z < 0 ? nextPlane.z : nextPlane.z-1
        );
    voxelizePoint(pos, color);


    float3 t = (nextPlane - v1) / unitDir;
    float3 tStep = 1.0f / abs(unitDir);
    float maxStep = abs(dir.x) + abs(dir.y) + abs(dir.z) + 3.0f;

    int i = 0;
    float stepLength = 0;

    while(pos != toInt3(v2) && stepLength < dirLength)
    {
      float tmin = min(t);
      int axis = 0;
      if(tmin == t.x) axis = 0;
      else if(tmin == t.y) axis = 1;
      else if(tmin == t.z) axis = 2;

      t -= tmin;
      stepLength += tmin;

      set(pos, axis, get(pos, axis) + get(signDir, axis));
      voxelizePoint(pos, color);
      set(t, axis, get(tStep, axis));
      i++;
    }
  }

  __device__ 
  void calculateScanline(float3 v1, float3 v2, float3 v3, int axis, ScanlineCache& cache)
  {
    float3 dir12 = v2 - v1;
    float3 dir13 = v3 - v1;
    float3 dir23 = v3 - v2;

    cache.unitDir12 = normalize(dir12);
    cache.unitDir13 = normalize(dir13);
    cache.unitDir23 = normalize(dir23);

    int axisX = (axis+1)%3; // Not really the x axis
    int axisY = (axis+2)%3; // Not really the y axis

    cache.projUnitDir12 = toFloat2(cache.unitDir12, axisX, axisY);
    cache.projUnitDir13 = toFloat2(cache.unitDir13, axisX, axisY);
    cache.projUnitDir23 = toFloat2(cache.unitDir23, axisX, axisY);

    float3 slDir;
    if(get(v1, axis) != get(v3, axis))
    {
      slDir = cross(dir12, dir13);
      float z = get(slDir, axis);
      slDir = -slDir * sign(z) / length(toFloat2(slDir, axisX, axisY));
      set(slDir, axis, abs(1/get(slDir, axis)));
    }
    else
    {
      slDir = dir13 / length(toFloat2(dir13, axisX, axisY));
    }

    // exact scanline length can cause missing scanlines due to rounding error
    cache.scanlineLength = (abs(get(slDir,axisX)) + abs(get(slDir, axisY))) * 0.999;

    float2 projSlDir = toFloat2(slDir, axisX, axisY);

    // Recalculate v2 so that the scanline always start from v1
    // I.e. make dot(p2-v2, projSlDir) = 0 
    float3 v2new = v2 - cache.unitDir23 * dot(projSlDir, toFloat2(dir12, axisX, axisY)) / dot(projSlDir, cache.projUnitDir23);
    dir23 = v3 - v2new;

    cache.v1 = v1;
    cache.v2 = v2new;
    cache.v3 = v3;
    cache.slInvDotDir12 = 1 / dot(projSlDir, cache.projUnitDir12);
    cache.slInvDotDir13 = 1 / dot(projSlDir, cache.projUnitDir13);
    cache.slInvDotDir23 = 1 / dot(projSlDir, cache.projUnitDir23);
    cache.lengthDir12 = length(dir12);
    cache.lengthDir13 = length(dir13);
    cache.minLengthDir23 = length(v2 - v2new);
    cache.lengthDir23 = length(v3 - v2new);
    cache.scanlineInvDirAxis = abs(1 / get(slDir, axis));
    cache.scanlineMaxLength = dot(projSlDir, toFloat2(dir13, axisX, axisY));
  }

  __device__
  void voxelizeScanline(const ScanlineCache& cache, float slLength, int axis, float height, unsigned char color)
  {
    float3 fromPos = cache.v1;
    float3 fromDir = cache.unitDir12;
    float invDot = cache.slInvDotDir12;

    if(slLength * cache.slInvDotDir12 >= cache.lengthDir12 || slLength * cache.slInvDotDir12 < 0)
    {
      // If this also out of range assume we are outside the triangle
      if(slLength * cache.slInvDotDir23 >= cache.lengthDir23 || 
          slLength * cache.slInvDotDir23 <= cache.minLengthDir23)
      {
        return;
      }

      fromPos = cache.v2;
      fromDir = cache.unitDir23;
      invDot = cache.slInvDotDir23;
    }

    float3 from = fromPos + fromDir * slLength * invDot;
    float3 to = cache.v1 + cache.unitDir13 * slLength * cache.slInvDotDir13;

    set(from, axis, height);
    set(to, axis, height);

    voxelizeLine(from, to, color);
  }

  __device__
  void voxelizeInterior(float3 v1, float3 v2, float3 v3, int axis)
  {
    float nextPlane = floor(get(v1, axis) + 1);

    ScanlineCache cache;
    calculateScanline(v1, v2, v3, axis, cache);

    float planeT = (nextPlane - get(v1, axis)) * cache.scanlineInvDirAxis;
    float t = cache.scanlineLength;

    float plane = nextPlane-0.5;

    // Triangle forms a line
    if(std::isnan(cache.scanlineMaxLength))
    {
      return;
    }

    while(true)
    {
      t = min(t, min(planeT, cache.scanlineMaxLength));
      if(t == cache.scanlineMaxLength) // Fully voxelized
        break;
      if(t == planeT) // New scanline slice 
      {
        voxelizeScanline(cache, t, axis, plane, 1);
        plane++;
        planeT += cache.scanlineInvDirAxis;
      }
      voxelizeScanline(cache, t, axis, plane, 1);
      t += cache.scanlineLength;
    }
  }

  __device__
  void voxelize(float3 v1, float3 v2, float3 v3)
  {
    float3 normal = abs(cross(v2 - v1, v3 - v1));
    float domNormal = max(normal);
    int axis = 0;
    if(domNormal == normal.x) axis = 0;
    else if(domNormal == normal.y) axis = 1;
    else if(domNormal == normal.z) axis = 2;

    // ---- Sort by most dominant axis
    if(get(v1, axis) > get(v2, axis))
      swap(v1,v2);
    if(get(v1, axis) > get(v3, axis))
      swap(v1,v3);
    if(get(v2, axis) > get(v3, axis))
      swap(v2,v3);


    voxelizeLine(v1, v2, 1);
    voxelizeLine(v2, v3, 1);
    voxelizeLine(v1, v3, 1);

    voxelizeInterior(v1, v2, v3, axis);
  }
}

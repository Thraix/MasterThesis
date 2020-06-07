#pragma once

#include "VectorMath.cu"

surface<void, 3> voxelGrid;
float* voxelGridArray;
__constant__ int voxelGridSize;
__constant__ Mat4 modelMatrix;

__shared__ bool compareFirst;

#ifdef _ERROR_ANALYSIS
__device__
void voxelizePoint(int3 v, unsigned char color)
{
  if(v.x >= 0 && v.x < voxelGridSize &&
      v.y >= 0 && v.y < voxelGridSize &&
      v.z >= 0 && v.z < voxelGridSize)
  {
    if(compareFirst)
    {
      color = 2;
    }
    else
    {
      unsigned char oldColor;
      surf3Dread(&oldColor, voxelGrid, v.x, v.y, v.z);
      if(oldColor == 3 || oldColor == 2)
        color = 3;
      else
        color = 4;
    }
    // My auto completer doesn't recognize surf3Dread or surf3Dwrite
    // So disable auto complete here
#ifndef __YCM__
    surf3Dwrite(color, voxelGrid, v.x, v.y, v.z);
#endif
  }
}
#else
__device__
void voxelizePoint(int3 v, unsigned char color)
{
  if(v.x >= 0 && v.x < voxelGridSize &&
      v.y >= 0 && v.y < voxelGridSize &&
      v.z >= 0 && v.z < voxelGridSize)
  {
    // My auto completer doesn't recognize surf3Dread or surf3Dwrite
    // So disable auto complete here
#ifndef __YCM__
    /* color = 1; */
    /* unsigned char oldColor; */
    /* surf3Dread(&oldColor, voxelGrid, v.x, v.y, v.z); */
    /* oldColor++; */
    /* color = oldColor; */
    /* if(oldColor == 0) */
    surf3Dwrite(color, voxelGrid, v.x, v.y, v.z);
#endif
  }
}
#endif

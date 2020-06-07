#include "Common.cu"

#if defined(_RLV)
#include "VoxelizeRLV.cu"
#else
#include "VoxelizeInteger.cu"
#endif

extern "C" __global__
void voxelize(int* indices, int indexCount, float3* vertices, int vertexCount, bool first)
{
  compareFirst = first;
  int index = threadIdx.x + blockIdx.x * blockDim.x;
  if(index * 3 + 2 > indexCount)
    return;
  int i1 = indices[index * 3];
  int i2 = indices[index * 3 + 1];
  int i3 = indices[index * 3 + 2];

  float3 v1 = modelMatrix * vertices[i1];
  float3 v2 = modelMatrix * vertices[i2];
  float3 v3 = modelMatrix * vertices[i3];

#if defined(_RLV)
    RLV::voxelize(v1, v2, v3);
#elif defined(_ILV)
    ILV::voxelize(v1, v2, v3);
#elif defined(_BRESENHAM)
    ILV::voxelize(v1, v2, v3);
#else
#error No Algorithm specified
#endif
}


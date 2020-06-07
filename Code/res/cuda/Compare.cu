surface<void, 3> voxelGrid;
__constant__ int voxelGridSize;

extern "C" __global__
void compare(int32_t* result)
{
  // This is not how you should code a GPU....
  // But downloading GB of data to the CPU RAM
  // gave me segfaults...
  int offsetDim = ceil(voxelGridSize / (float)blockDim.x);
  int index = threadIdx.x * offsetDim;
  int intersection = 0;
  int inFirst = 0;
  int inSecond = 0;
  for(int z = index;z<index+offsetDim && z<voxelGridSize;z++)
  {
    for(int y = 0;y<voxelGridSize;y++)
    {
      for(int x = 0;x<voxelGridSize;x++)
      {
        unsigned char color;
        surf3Dread(&color, voxelGrid, x, y, z);
        if(color == 3)
          intersection++;
        else if(color == 2)
          inFirst++;
        else if(color == 4)
          inSecond++;
        else if(color != 0)
          printf("Unmatched %d!\n", color);
      }
    }
  }
  atomicAdd(&result[0], intersection);
  atomicAdd(&result[1], inFirst);
  atomicAdd(&result[2], inSecond);
}

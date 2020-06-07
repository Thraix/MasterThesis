#pragma once

#include <stdio.h>

#include <thread>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <cuda.h>
#include <cudaGL.h>
#include "CudaUtils.h"
#include <cstring>
#include <logging/Log.h>
#include <utils/Timer.h>
#include <graphics/models/MeshData.h>

namespace Voxelizer
{

  enum class VoxAlg
  {
    RLV=0, ILV=1, Bresenham=2
  };

  struct VoxelKernel
  {
    CUmodule module;
    CUfunction function;

    CUgraphicsResource voxelGridTex;
    CUarray voxelGridArray;
    CUsurfref voxelGridSurfRef;

    CUdeviceptr devIndices;
    CUdeviceptr devVertices;
  };

  struct CompareKernel
  {
    CUmodule module;
    CUfunction function;

    CUgraphicsResource voxelGridTex;
    CUarray voxelGridArray;
    CUsurfref voxelGridSurfRef;

    CUdeviceptr result;
  };

  static CUcontext context;
  static CUdevice device;
  static uint32_t indexCount;
  static uint32_t vertexCount;

  static VoxelKernel kernels[2];
  static CompareKernel compareKernel;
  static int voxelGridSize;

  static void InitCuda()
  {
    using namespace Greet;

    CudaCall(cuInit(0));
    int devID = 0;

    // Otherwise pick the device with highest Gflops/s
    char name[100];
    devID = 0;
    CudaCall(cuDeviceGet(&device, devID));
    CudaCall(cuDeviceGetName(name, 100, device));

    int version;
    CudaCall(cuDriverGetVersion(&version));

    float versionFloat = version / 1000 + ((float)version / 100 - version / 100);
    int minorVersion = version % 100;
    Log::Info("CUDA version: ", versionFloat, " ", name);

    CudaCall(cuGLCtxCreate(&context, CU_CTX_SCHED_AUTO, device));
  }

  static void UploadMesh(const Greet::MeshData& mesh)
  {
    indexCount = mesh.GetIndexCount();
    vertexCount = mesh.GetVertexCount();

    // ---- Upload indices
    for(VoxelKernel& kernel : kernels)
    {
      CudaCall(cuMemAlloc(&kernel.devIndices, indexCount * sizeof(uint32_t)));
      CudaCall(cuMemcpyHtoD(kernel.devIndices, mesh.GetIndices().Data(), indexCount * sizeof(uint32_t)));

      // ---- Upload vertices
      CudaCall(cuMemAlloc(&kernel.devVertices, vertexCount * 3 * sizeof(float)));
      CudaCall(cuMemcpyHtoD(kernel.devVertices, mesh.GetVertices().Data(), vertexCount * 3 * sizeof(uint32_t)));
    }
  }

  static void UploadMat4(const Greet::Mat4& mat, const std::string& location)
  {
    CUdeviceptr  devMat4;
    size_t       devMat4Bytes;
    for(VoxelKernel& kernel : kernels)
    {
      CudaCall(cuModuleGetGlobal(&devMat4,  &devMat4Bytes, kernel.module, location.c_str()));
      CudaCall(cuMemcpyHtoD(devMat4, (void*)&mat, devMat4Bytes));
    }
  }

  static void InitKernel(uint32_t _voxelGridSize, VoxAlg primary, VoxAlg secondary,  bool compare)
  {
    voxelGridSize = _voxelGridSize;
    // Determine filename
    std::string sourcePrimary = "bin/intermediates/Voxelize";
    std::string sourceSecondary = "bin/intermediates/Voxelize";
    if(primary == VoxAlg::RLV) sourcePrimary += "RLV";
    else if(primary == VoxAlg::ILV) sourcePrimary += "ILV";
    else if(primary == VoxAlg::Bresenham) sourcePrimary += "Bresenham";
    if(secondary == VoxAlg::RLV) sourceSecondary += "RLV";
    else if(secondary == VoxAlg::ILV) sourceSecondary += "ILV";
    else if(secondary == VoxAlg::Bresenham) sourceSecondary += "Bresenham";
    if(compare)
    {
      sourcePrimary += "_comp";
      sourceSecondary += "_comp";
    }
    sourcePrimary += ".ptx";
    sourceSecondary += ".ptx";

    // ------ Load CUDA program
    CudaCall(cuModuleLoad(&kernels[0].module, sourcePrimary.c_str()));
    CudaCall(cuModuleLoad(&kernels[1].module, sourceSecondary.c_str()));
    CudaCall(cuModuleLoad(&compareKernel.module, "bin/intermediates/Compare.ptx"));

    for(VoxelKernel& kernel : kernels)
    {
      CudaCall(cuModuleGetFunction(&kernel.function, kernel.module, "voxelize"));

      CUdeviceptr  devVoxelGridSize;
      size_t       devVoxelGridSizeBytes;
      CudaCall(cuModuleGetGlobal(&devVoxelGridSize,  &devVoxelGridSizeBytes, kernel.module, "voxelGridSize"));
      CudaCall(cuMemcpyHtoD(devVoxelGridSize, (void*)&voxelGridSize, devVoxelGridSizeBytes));
    }

    CudaCall(cuModuleGetFunction(&compareKernel.function, compareKernel.module, "compare"));

    CUdeviceptr  devVoxelGridSize;
    size_t       devVoxelGridSizeBytes;
    CudaCall(cuModuleGetGlobal(&devVoxelGridSize,  &devVoxelGridSizeBytes, compareKernel.module, "voxelGridSize"));
    CudaCall(cuMemcpyHtoD(devVoxelGridSize, (void*)&voxelGridSize, devVoxelGridSizeBytes));
    CudaCall(cuMemAlloc(&compareKernel.result, 3 * sizeof(uint64_t)));
  }

  static void BindTexture(uint32_t _voxelGridTex)
  {
    for(VoxelKernel& kernel : kernels)
    {
      // ------ Link OpenGL texture with CUDA
      CudaCall(cuGraphicsGLRegisterImage(&kernel.voxelGridTex, _voxelGridTex, GL_TEXTURE_3D, CU_GRAPHICS_REGISTER_FLAGS_SURFACE_LDST));

      CudaCall(cuGraphicsMapResources(1, &kernel.voxelGridTex, 0));
      CudaCall(cuGraphicsSubResourceGetMappedArray(&kernel.voxelGridArray, kernel.voxelGridTex, 0, 0));
      CudaCall(cuGraphicsUnmapResources(1, &kernel.voxelGridTex, 0));

      // ------ Link GPU objects to the CPU
      CudaCall(cuModuleGetSurfRef(&kernel.voxelGridSurfRef, kernel.module, "voxelGrid"));
      CudaCall(cuSurfRefSetArray(kernel.voxelGridSurfRef, kernel.voxelGridArray, 0));
    }

    // ------ Link OpenGL texture with CUDA
    CudaCall(cuGraphicsGLRegisterImage(&compareKernel.voxelGridTex, _voxelGridTex, GL_TEXTURE_3D, CU_GRAPHICS_REGISTER_FLAGS_SURFACE_LDST));

    CudaCall(cuGraphicsMapResources(1, &compareKernel.voxelGridTex, 0));
    CudaCall(cuGraphicsSubResourceGetMappedArray(&compareKernel.voxelGridArray, compareKernel.voxelGridTex, 0, 0));
    CudaCall(cuGraphicsUnmapResources(1, &compareKernel.voxelGridTex, 0));

    // ------ Link GPU objects to the CPU
    CudaCall(cuModuleGetSurfRef(&compareKernel.voxelGridSurfRef, compareKernel.module, "voxelGrid"));
    CudaCall(cuSurfRefSetArray(compareKernel.voxelGridSurfRef, compareKernel.voxelGridArray, 0));
  }

  static float RunVoxelize(bool compare)
  {
    bool first = true;
    VoxelKernel& kernel = kernels[0];
    void* args[5] = {&kernel.devIndices, &indexCount, &kernel.devVertices, &vertexCount, &first};

    uint totalThreads = indexCount / 3; // On for each triangle
    int threadsPerBlock = 32;
    int blocksPerGrid = (totalThreads + threadsPerBlock-1) / threadsPerBlock;

    int i = 0;
    float time;
    CUevent start, stop;

    CudaCall(cuEventCreate(&start, CU_EVENT_DEFAULT) );
    CudaCall(cuEventCreate(&stop, CU_EVENT_DEFAULT) );
    CudaCall(cuEventRecord(start, 0) );

    // ------ Run CUDA kernel
    CudaCall(cuLaunchKernel(kernel.function,
          blocksPerGrid, 1, 1, // Grid dimension
          threadsPerBlock, 1, 1, // Block dimension
          0, NULL, args, NULL));
    CudaCall( cuEventRecord(stop, 0) );
    CudaCall( cuEventSynchronize(stop) );
    CudaCall( cuEventElapsedTime(&time, start, stop) );
    CudaCall(cuEventDestroy(start));
    CudaCall(cuEventDestroy(stop));
/* #define _ERROR_ANALYSIS */
    if(compare)
    {
      first = false;
      kernel = kernels[1];
      args[0] = &kernel.devIndices;
      args[2] = &kernel.devVertices;
      CudaCall(cuLaunchKernel(kernel.function,
            blocksPerGrid, 1, 1, // Grid dimension
            threadsPerBlock, 1, 1, // Block dimension
            0, NULL, args, NULL));

      void* argsCompare[3] = {&compareKernel.result};
      CudaCall(cuLaunchKernel(compareKernel.function,
            1,1,1,
            256,1,1,
            0, NULL, argsCompare, NULL));

      CudaCall(cuCtxSynchronize());
    }
    return time;
  }

  static float GetJaccard()
  {
    int32_t data[3];
    CudaCall(cuMemcpyDtoH(&data, compareKernel.result, 3 * sizeof(int32_t)));
    Greet::Log::Info(data[0], " ", data[1], " ", data[2]);
    return (data[1] + data[2]) / (float)(data[0] + data[1] + data[2]);
  }

  static float GetRelative()
  {
    int32_t data[3];
    CudaCall(cuMemcpyDtoH(&data, compareKernel.result, 3 * sizeof(int32_t)));
    return (data[2] - data[1]) / (float)(data[0] + data[1]);
  }
}

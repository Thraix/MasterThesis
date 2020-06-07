#pragma once

#ifdef _DEBUG
#include <GL/glew.h>
#include <cuda.h>
#include <cudaGL.h>
#include <cstdlib>
#include <iostream>
#define CudaCall(x) cudaAssert(x, __LINE__, __FILE__, #x)
static void cudaAssert(CUresult code, uint32_t line, const char* filename, const char* functionName)
{
  if(code != CUDA_SUCCESS)
  {
    std::cout << "CUDA ERROR(" << code << ") " << filename << ":" << line << " at " << functionName << std::endl;
    abort();
  }
}
#else
#define CudaCall(x) x
#endif


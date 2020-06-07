#pragma once

#include <common/Types.h>
#include <vector>
#include <iostream>

namespace Greet {

  struct NoiseData3D
  {
    uint width,height,length;
    std::vector<float> result;
    std::vector<float> smoothCache;

    float& SmoothCache(uint x, uint y, uint z) {
      return smoothCache[x + y * (width+1) + z * (width+1) * (height+1)];
    }
  };

  class Noise
  {

    public:

    private:
      Noise() = delete;
      Noise(const Noise& noise) = delete;

    public:
      /*
       * width = width of the noise
       * height = height of the noise
       * octave = number of iterations the noise should go through
       * stepX = number of pixels in the first iteration in the x axis
       * stepY = number of pixels in the first iteration in the y axis
       */
      static std::vector<float> GenNoise(uint width, uint height, uint octave, uint stepX, uint stepY, float persistance, int offsetX=0, int offsetY=0);

      // 3D
      static std::vector<float> GenNoise(uint width, uint height, uint length, uint octave, uint stepX, uint stepY, uint stepZ, float persistance, int offsetX, int offsetY, int offsetZ);
    private:
      static float Smooth(int x, int y, uint octave);
      static float Eval(int x, int y, uint width, uint height, float stepX, float stepY, uint octaves, float persistance);

      // 3D
      static float Smooth(int x, int y, int z, int xCache, int yCache, int zCache,  uint octave, NoiseData3D& data);
    public:
      static float PRNG(int x, int y, int z, int octave);
      static float PRNG(int x, int y, int octave);

      static float Interpolate(float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float d1, float d2, float d3);
      static float Interpolate(float v1, float v2, float v3, float v4, float d1, float d2);
      static float Interpolate(float v1, float v2, float d);
  };
}

#include "Noise.h"
#include <string>
#include <cmath>
#include <cstring>
#include <logging/Log.h>
#include <math/MathFunc.h>
#include <unordered_map>

namespace Greet {

  float Noise::Eval(int x, int y,uint width, uint height, float stepX, float stepY, uint octaves, float persistance)
  {

    float totalPersistance = 0;
    float p = persistance;
    // Calculate the total persistance to normalize the end value
    for(int i = 0;i<octaves;i++)
    {
      totalPersistance += p;
      p*=p;
    }
    float value = 0.0f;

    for(int i = 0;i<octaves;i++)
    {
      // Convert to 1 step grid system.
      float xx = x / stepX;
      float yy = y / stepY;

      // Retrieve the 4 noise points round the "pixel"
      float tl = Smooth(xx, yy, i);
      float tr = Smooth(xx+1, yy, i);
      float bl = Smooth(xx, yy+1, i);
      float br = Smooth(xx+1, yy+1, i);

      // Interpolate the 4 values
      value += Interpolate(tl, tr, bl, br, xx - (int)xx, yy - (int)yy) * persistance;

      // Decrease the step by half
      // This should maybe be a parameter in the future
      stepX *= 0.5f;
      stepY *= 0.5f;
      persistance *= persistance;
    }
    return value / totalPersistance;
  }

  float Noise::Smooth(int x, int y, uint octave)
  {
    // Smooth values ( center * 0.5 + edges * 0.08333 + corners * 0.0416666)
    return PRNG(x, y, octave) * 0.5 +
      (PRNG(x-1, y, octave) + PRNG(x+1, y, octave) + PRNG(x, y-1, octave) + PRNG(x, y+1, octave)) * 0.08333f +
      (PRNG(x-1, y-1, octave) + PRNG(x+1, y-1, octave) + PRNG(x-1, y+1, octave) + PRNG(x+1, y+1, octave)) * 0.0416666f;
  }

  float Noise::Smooth(int x, int y, int z, int xCache, int yCache, int zCache, uint octave, NoiseData3D& data)
  {
    float smoothCache = data.SmoothCache(xCache,yCache,zCache);
    if(smoothCache)
      return smoothCache;

    double result = 0.0f;
    for(int i = -1;i<=1;i++)
    {
      for(int j = -1;j<=1;j++)
      {
        for(int k = -1;k<=1;k++)
        {
          //float randomCache = data.randomCache[data.Index(x+k,y+j,z+i)];
          //float noise = randomCache;
          float noise = 0.0f;
          if(!noise)
          {
            noise = PRNG(x+k, y+j, z+i, octave);
            //data.randomCache[data.Index(x+k,y+j,z+i)] = noise;
          }
          int manhattanDistance = abs(i) + abs(j) + abs(k);
          switch(manhattanDistance)
          {
            case 0: // center
              result += noise * 0.5;
              break;
            case 1: // edges
              result += noise * 1.0 / 24.0;
              break;
            case 2: // edge-corners
              result += noise * 1.0 / 80.0;
              break;
            case 3: // corners
              result += noise * 1.0 / 80.0;
              break;
          }
        }
      }
    }
    data.SmoothCache(xCache,yCache,zCache) = result;
    return result;
  }

  std::vector<float> Noise::GenNoise(uint width, uint height, uint length, uint octaves, uint stepX, uint stepY, uint stepZ, float persistance, int offsetX, int offsetY, int offsetZ)
  {
    std::vector<float> result(width * height * length);
    std::vector<float> smoothCache((width+1) * (height+1) * (length+1));
    std::fill(result.begin(), result.end(), 0);
    std::fill(smoothCache.begin(), smoothCache.end(), 0);

    NoiseData3D data;
    data.width = width;
    data.height = height;
    data.length = length;
    data.result = result;
    data.smoothCache = smoothCache;
    //data.randomCache = randomCache;

    float totalPersistance = 0;
    float p = persistance;
    // Calculate the total persistance to normalize the end value
    for(int i = 0;i<octaves;i++)
    {
      totalPersistance += p;
      p*=p;
    }

    float sX = stepX,sY = stepY,sZ = stepZ;
    // Calculate the noise for each "pixel"
    // Could be improved by not computing the same noise values multiple timers
    // Retrieve the 4 noise points round the "pixel"
    for(int i = 0;i<octaves;i++)
    {
      float persistanceNorm = persistance / totalPersistance;
      for(int z = 0;z<length;z++)
      {
        for(int y = 0;y<height;y++)
        {
          for(int x = 0;x<width;x++)
          {
            // Convert to 1 step grid system.
            float xx = (x + offsetX) / sX;
            float yy = (y + offsetY) / sY;
            float zz = (z + offsetZ) / sZ;

            float xCache = x / sX;
            float yCache = y / sY;
            float zCache = z / sZ;

            float btl = Smooth(xx  , yy  , zz  , xCache  , yCache  , zCache  , i, data);
            float btr = Smooth(xx+1, yy  , zz  , xCache+1, yCache  , zCache  , i, data);
            float bbl = Smooth(xx  , yy+1, zz  , xCache  , yCache+1, zCache  , i, data);
            float bbr = Smooth(xx+1, yy+1, zz  , xCache+1, yCache+1, zCache  , i, data);
            float ftl = Smooth(xx  , yy  , zz+1, xCache  , yCache  , zCache+1, i, data);
            float ftr = Smooth(xx+1, yy  , zz+1, xCache+1, yCache  , zCache+1, i, data);
            float fbl = Smooth(xx  , yy+1, zz+1, xCache  , yCache+1, zCache+1, i, data);
            float fbr = Smooth(xx+1, yy+1, zz+1, xCache+1, yCache+1, zCache+1, i, data);


            // Interpolate the 8 values
            float value = Interpolate(btl, btr, bbl, bbr, ftl, ftr, fbl, fbr, xx - (int)xx, yy - (int)yy, zz - (int)zz) * persistanceNorm;

            result[x + y * width + z * width * height] += value;
          }
        }
      }

      persistance *= persistance;
      sX *= 0.5f;
      sY *= 0.5f;
      sZ *= 0.5f;

      // Reset the cache
      std::fill(data.smoothCache.begin(), data.smoothCache.end(), 0);
    }
    return result;
  }

  std::vector<float> Noise::GenNoise(uint width, uint height, uint octave, uint stepX, uint stepY, float persistance, int offsetX, int offsetY)
  {
    std::vector<float> result = std::vector<float>(width * height);

    // Calculate the noise for each "pixel"
    // Could be improved by not computing the same noise values multiple timers
    for(int y = 0;y<height;y++)
    {
      for(int x = 0;x<width;x++)
      {
        result[x + y * width] = Eval(x + offsetX, y + offsetY, width, height, stepX, stepY, octave, persistance);
      }
    }
    return result;
  }

  float Noise::PRNG(int x, int y, int octave)
  {
    // hash the values to get a better "random" n value
    size_t n = std::hash<int>()(x * 34184077) ^ std::hash<int>()(y * 15487067) ^ std::hash<int>()(octave * 87855217);
    return (double)( ( n * (n * n * 20947 + 794327) + 1376312589) & 0x7fffffff) / 1073741823.0*0.5;
  }

  float Noise::PRNG(int x, int y, int z, int octave)
  {
    // hash the values to get a better "random" n value
    size_t n =
      std::hash<int>()(x * 34184077) ^
      std::hash<int>()(y * 15487067) ^
      std::hash<int>()(z * 67867967) ^
      std::hash<int>()(octave * 87855217);
    return (double)( ( n * (n * n * 20947 + 794327) + 1376312589) & 0x7fffffff) / 1073741823.0*0.5;
  }

  // 3d interpolation
  float Noise::Interpolate(float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float d1, float d2, float d3)
  {
    return Interpolate(Interpolate(Interpolate(v1, v2, d1), Interpolate(v3, v4, d1), d2),Interpolate(Interpolate(v5, v6, d1), Interpolate(v7, v8, d1), d2), d3);
  }
  // 2d interpolation
  float Noise::Interpolate(float v1, float v2, float v3, float v4, float d1, float d2)
  {
    return Interpolate(Interpolate(v1, v2, d1), Interpolate(v3, v4, d1), d2);
  }

  // 1d interpolation
  float Noise::Interpolate(float v1, float v2, float d)
  {
    float c = 0.5f+cos(d * M_PI) * 0.5f;
    float v = (v1 - v2)*c + v2;

    return v;
  }
}

#pragma once

#include <math.h>
#include <common/Types.h>
#include <iostream>


namespace Greet{ namespace Math {

  template <typename T>
  inline T Max(const T& t1, const T& t2)
  {
    return t1 > t2 ? t1 : t2;
  }

  template <typename T>
  inline T Min(const T& t1, const T& t2)
  {
    return t1 < t2 ? t1 : t2;
  }

  inline float RoundDown(float numToRound, float multiple)
  {
    if (multiple == 0)
      return 0;
    return ((int)(numToRound / multiple))*multiple;
  }

  inline float RoundUp(float numToRound, float multiple)
  {
    if (multiple == 0)
      return 0;
    float ans = ((int)(numToRound / multiple))*multiple;
    return ans < numToRound ? ans + multiple : ans;
  }

  inline float RoundClose(float numToRound, float multiple)
  {
    if (multiple == 0)
      return numToRound;
    float down = ((int)(numToRound / multiple))*multiple;
    float up = down < numToRound ? down + multiple : down;
    return (numToRound - down) < (up - numToRound) ? down : up;
  }

  inline bool IsPositive(float val)
  {
    return -val < val;
  }

  //struct Vec2;
  template<typename T>
  inline void Clamp(T* value, const T& min, const T& max)
  {
    if(min < max)
      *value = *value < min ? min : (*value > max ? max : *value);
    else
      *value = *value < max ? max : (*value > min ? min : *value);
  }

  // Returns half the value, add one to value if it is odd.
  inline int Half(int value)
  {
    return (value + (value % 2)) / 2;
  }


  inline float ToRadians(float degrees)
  {
    return (float)(degrees * M_PI * 0.005555555f);
  }

  inline float ToDegrees(float radians)
  {
    return (float)(radians * 180.0f / M_PI);
  }
}}

#pragma once

#include <string>
#include <ctime>
#include <common/Types.h>

#define BIT(x) (1 << (x))

namespace Greet {

  struct Utils
  {
    template<class T> struct ptr_less
    {
      bool operator()(const T* lhs, const T* rhs) const
      {
        return *lhs < *rhs;
      }
    };

    static const std::string CurrentDateTime()
    {
      time_t now = time(0);
      struct tm tstruct;
      char buf[80];
      tstruct = *localtime(&now);
      strftime(buf,sizeof(buf),"%Y%m%d%H%M%S",&tstruct);
      return buf;
    }

    static void Screenshot(uint width, uint height, std::string filename = "");

    template<typename Base, typename T>
    inline bool InstanceOf(const T *ptr) {
      return dynamic_cast<const Base*>(ptr) != nullptr;
    }

    static std::string GetTime()
    {
      time_t currentTime;
      struct tm *localTime;
      time(&currentTime);
      localTime = localtime(&currentTime);
      int hour = localTime->tm_hour;
      int min = localTime->tm_min;
      std::string res = "";
      if (hour < 10)
        res += "0";
      res += std::to_string(hour)+":";
      if (min < 10)
        res += "0";
      res += std::to_string(min);
      return res;
    }

    static byte* GetBytesFromString(const std::string& s)
    {
      byte* bytes = new byte[s.length()];
      for(uint i = 0;i<s.length();i++)
      {
        bytes[i] = s[i];
      }
      return bytes;
    }

    static uint HSVtoARGB(float hue, float sat, float val)
    {
      int h = (int)(hue * 6.0f);
      float f = hue * 6.0f - h;
      float p = val* (1.0f - sat);
      float q = val* (1.0f - f*sat);
      float t = val* (1.0f - (1.0f - f)*sat);
      float r = 0;
      float g = 0;
      float b = 0;
      switch (h) {
        case 0:
          r = val;
          g = t;
          b = p;
          break;
        case 1:
          r = q;
          g = val;
          b = p;
          break;
        case 2:
          r = p;
          g = val;
          b = t;
          break;
        case 3:
          r = p;
          g = q;
          b = val;
          break;
        case 4:
          r = t;
          g = p;
          b = val;
          break;
        case 5:
          r = val;
          g = p;
          b = q;
          break;
      }
      return 0xff000000 + ((int)(r * 255.0f) << 16) + ((int)(g * 255.0f) << 8) + (int)(b * 255.0f);
    }

    static unsigned long long powi(uint b, uint n)
    {
      unsigned long long r = 1;
      for (uint i = 0; i < n; i++)
      {
        r *= b;
      }
      return r;
    }
  };
}

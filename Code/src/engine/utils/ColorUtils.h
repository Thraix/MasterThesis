#pragma once

#include <common/Types.h>
#include <math/Maths.h>
#include <logging/Logger.h>

#define COLOR_UTILS_MODIFIER 0.1f

namespace Greet {

  struct ColorUtils
  {
    static Vec4 HSVtoRGB(float h, float s, float v, float a)
    {
      if(h == 1.0)
        h = 0;
      float c = v * s;
      float h6f = h*6.0f;
      uint h6 = (uint)h6f;

      float f = h6f - h6;

      float p = v * (1 - s);
      float q = v * (1 - s * f);
      float t = v * (1 - s * (1 - f));

      switch (h6)
      {
        case 0:
          return Vec4(v, t, p, a);
        case 1:
          return Vec4(q, v, p, a);
        case 2:
          return Vec4(p, v, t, a);
        case 3:
          return Vec4(p, q, v, a);
        case 4:
          return Vec4(t, p, v, a);
        case 5:
          return Vec4(v, p, q, a);
        default:
          return Vec4(c, c, c, a);
      }
    }

    static Vec4 HSVtoRGB(const Vec4& hsv)
    {
      return HSVtoRGB(hsv.x,hsv.y,hsv.z, hsv.w);
    }

    static Vec4 RGBtoHSV(float r, float g, float b, float a)
    {
      float cmin = r < g ? (r < b ? r : b) : (g < b ? g : b);
      float cmax = r > g ? (r > b ? r : b) : (g > b ? g : b);

      float delta = cmax - cmin;
      Vec4 hsva{0,0,cmax,a};
      if (cmax != 0)
        hsva.s = delta / cmax;
      if (delta != 0)
      {
        if (cmax == r)
        {
          hsva.h = (g - b) / delta * 0.16667;
        }
        else if (cmax == g)
        {
          hsva.h = ((b - r) / delta + 2) * 0.16667;
        }
        else // if (cmax == b)
        {
          hsva.h = ((r - g) / delta + 4) * 0.16667;
        }

        if(hsva.h < 0)
          hsva.h++;
      }
      return hsva;
    }

    static Vec4 RGBtoHSV(uint colorHex)
    {
      return RGBtoHSV(((colorHex & 0xff0000) >> 16)/255.0f, ((colorHex & 0xff00) >> 8) / 255.0f, (colorHex & 0xff) / 255.0f, ((colorHex & 0xff0000) >> 24) / 255.0f);
    }

    static Vec4 RGBtoHSV(Vec4 rgb)
    {
      return RGBtoHSV(rgb.x,rgb.y,rgb.z,rgb.w);
    }

    static uint Vec3ToColorHex(const Vec3<float>& color)
    {
      return 0xff000000 | ((int)(color.x * 255) << 16) | ((int)(color.y * 255) << 8) | (int)(color.z * 255);
    }

    static Vec4 ColorHexToVec4(uint color)
    {
      return Vec4((color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff, (color >> 24) & 0xff) / 255.0f;
    }

    static byte GetAlpha(uint color)
    {
      return (color >> 24) & 0xff;
    }

    static byte GetRed(uint color)
    {
      return (color >> 16) & 0xff;
    }

    static byte GetGreen(uint color)
    {
      return (color >> 8) & 0xff;
    }

    static byte GetBlue(uint color)
    {
      return color & 0xff;
    }

    static Vec4 HexToVec4(uint color)
    {
      return Vec4(GetRed(color), GetGreen(color), GetBlue(color), GetAlpha(color)) / 255.0f;
    }

    static uint Vec4ToColorHex(Vec4 color)
    {
      return ((int)(color.w * 255) << 24) | ((int)(color.x * 255) << 16) | ((int)(color.y * 255) << 8) | (int)(color.z * 255);
    }

    static Vec4 GetMaterialColorAsHSV(float hue, int level)
    {
      float p = powf(1.0f - COLOR_UTILS_MODIFIER, level);
      return Vec4(hue,p,p,1);
    }

    static Vec4 GetMaterialColorAsRGB(float hue, int level)
    {
      float p = powf(1.0f-COLOR_UTILS_MODIFIER, level);
      return HSVtoRGB(hue,p,p,1.0);
    }
  };
}

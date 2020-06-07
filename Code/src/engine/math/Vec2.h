#pragma once

#include <cmath>
#include <iostream>
#include <math/Vec3.h>
#include <math/Vec4.h>

namespace Greet{

  struct Vec2
  {
    public:
      union
      {
        struct
        {
          float x, y;
        };
        struct
        {
          float w, h;
        };
        struct
        {
          float s, t;
        };
      };
      Vec2();
      Vec2(float x, float y);
      Vec2(const Vec3<float>& vec3);
      Vec2(const Vec4& vec4);

      float Length();
      float LengthSQ();
      float Dot(const Vec2& vec) const;
      float Cross(const Vec2& vec) const;
      Vec2& Abs();
      Vec2& Projected(const Vec2& vec);
      Vec2& Normalize();
      Vec2& Rotate(const float deg);
      Vec2& RotateR(const float rad);

      Vec2& Add(const Vec2& other);
      Vec2& Subtract(const Vec2& other);
      Vec2& Multiply(const Vec2& other);
      Vec2& Divide(const Vec2& other);

      Vec2& Add(const float other);
      Vec2& Subtract(const float other);
      Vec2& Multiply(const float other);
      Vec2& Divide(const float other);

      bool Compare(const Vec2& other);

      friend Vec2 operator+(const Vec2& first, const Vec2 &second);
      friend Vec2 operator-(const Vec2& first, const Vec2 &second);
      friend Vec2 operator*(const Vec2& first, const Vec2 &second);
      friend Vec2 operator/(const Vec2& first, const Vec2 &second);

      friend Vec2 operator+(const Vec2& first, const float c);
      friend Vec2 operator-(const Vec2& first, const float c);
      friend Vec2 operator-(const Vec2& first);
      friend Vec2 operator*(const Vec2& first, const float c);
      friend Vec2 operator/(const Vec2& first, const float c);

      float operator[](uint i);

      Vec2& operator+=(const Vec2 &other);
      Vec2& operator-=(const Vec2 &other);
      Vec2& operator*=(const Vec2 &other);
      Vec2& operator/=(const Vec2 &other);

      Vec2& operator+=(const float c);
      Vec2& operator-=(const float c);
      Vec2& operator*=(const float c);
      Vec2& operator/=(const float c);


      bool operator!=(const Vec2 &other);
      bool operator==(const Vec2 &other);

      bool operator<(const Vec2& other);
      bool operator>(const Vec2& other);
      bool operator<=(const Vec2& other);
      bool operator>=(const Vec2& other);

      friend std::ostream& operator<<(std::ostream& stream, const Vec2& vec);

      inline float toRadians(float degrees)
      {
        return degrees * ((float)M_PI / 180.0f);
      }
  };

}

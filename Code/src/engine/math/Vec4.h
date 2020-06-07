#pragma once

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <iostream>

namespace Greet{

  struct Vec4
  {
    union
    {
      float vals[4];
      struct {
        float x, y, z, w;
      };
      struct {
        float r, g, b, a;
      };
      struct {
        float h, s, v, hsv_a;
      };
      struct {
        float top, left, bottom, right;
      };
    };
    Vec4();
    //Vec4(const Vec4& copy);
    Vec4(float x, float y, float z, float w);

    float Length();

    Vec4& Add(const Vec4& other);
    Vec4& Subtract(const Vec4& other);
    Vec4& Multiply(const Vec4& other);
    Vec4& Divide(const Vec4& other);

    Vec4& Add(const float other);
    Vec4& Subtract(const float other);
    Vec4& Multiply(const float other);
    Vec4& Divide(const float other);

    bool Compare(const Vec4& other);

    friend Vec4 operator+(const Vec4& first, const Vec4 &second);
    friend Vec4 operator-(const Vec4& first, const Vec4 &second);
    friend Vec4 operator*(const Vec4& first, const Vec4 &second);
    friend Vec4 operator/(const Vec4& first, const Vec4 &second);

    friend Vec4 operator+(const Vec4& first, const float c);
    friend Vec4 operator-(const Vec4& first, const float c);
    friend Vec4 operator*(const Vec4& first, const float c);
    friend Vec4 operator/(const Vec4& first, const float c);

    float operator[](uint i);

    Vec4& operator+=(const Vec4 &other);
    Vec4& operator-=(const Vec4 &other);
    Vec4& operator*=(const Vec4 &other);
    Vec4& operator/=(const Vec4 &other);

    bool operator!=(const Vec4 &second);
    bool operator==(const Vec4 &second);

    friend std::ostream& operator<<(std::ostream& stream, const Vec4& vec);

  };
}

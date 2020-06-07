#include "Vec4.h"

#include <math.h>
#include <cassert>

namespace Greet{

  Vec4::Vec4()
  {
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->w = 0;
  }

  Vec4::Vec4(float x, float y, float z, float w)
  {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
  }

  float Vec4::Length()
  {
    return sqrt(x*x + y*y + z*z+ w*w);
  }

  Vec4& Vec4::Add(const Vec4& other)
  {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
  }

  Vec4& Vec4::Subtract(const Vec4& other)
  {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
    return *this;
  }

  Vec4& Vec4::Multiply(const Vec4& other)
  {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    w *= other.w;
    return *this;
  }

  Vec4& Vec4::Divide(const Vec4& other)
  {
    x /= other.x;
    y /= other.y;
    z /= other.z;
    w /= other.w;
    return *this;
  }

  Vec4& Vec4::Add(const float c)
  {
    x += c;
    y += c;
    z += c;
    w += c;
    return *this;
  }

  Vec4& Vec4::Subtract(const float c)
  {
    x -= c;
    y -= c;
    z -= c;
    w -= c;
    return *this;
  }

  Vec4& Vec4::Multiply(const float c)
  {
    x *= c;
    y *= c;
    z *= c;
    w *= c;
    return *this;
  }

  Vec4& Vec4::Divide(const float c)
  {
    x /= c;
    y /= c;
    z /= c;
    w /= c;
    return *this;
  }

  bool Vec4::Compare(const Vec4& other)
  {
    return x == other.x && y == other.y && z == other.z && w == other.w;
  }

  Vec4 operator+(const Vec4& first, const Vec4 &second)
  {
    return Vec4(first).Add(second);
  }

  Vec4 operator-(const Vec4& first, const Vec4 &second)
  {
    return Vec4(first).Subtract(second);
  }

  Vec4 operator*(const Vec4& first, const Vec4 &second)
  {
    return Vec4(first).Multiply(second);
  }

  Vec4 operator/(const Vec4& first, const Vec4 &second)
  {
    return Vec4(first).Divide(second);
  }

  Vec4 operator+(const Vec4& first, const float c)
  {
    return Vec4(first).Add(c);
  }

  Vec4 operator-(const Vec4& first, const float c)
  {
    return Vec4(first).Subtract(c);
  }

  Vec4 operator*(const Vec4&  first, const float c)
  {
    return Vec4(first).Multiply(c);
  }

  Vec4 operator/(const Vec4& first, const float c)
  {
    return Vec4(first).Divide(c);
  }

  float Vec4::operator[](uint i)
  {
    assert(i < 4);
    return *((&x)+i);
  }

  Vec4& Vec4::operator+=(const Vec4 &other)
  {
    return Add(other);
  }

  Vec4& Vec4::operator-=(const Vec4 &other)
  {
    return Subtract(other);
  }

  Vec4& Vec4::operator*=(const Vec4 &other)
  {
    return Multiply(other);
  }

  Vec4& Vec4::operator/=(const Vec4 &other)
  {
    return Divide(other);
  }

  bool Vec4::operator==(const Vec4 &other)
  {
    return Compare(other);
  }

  bool Vec4::operator!=(const Vec4 &other)
  {
    return !Compare(other);
  }

  std::ostream& operator<<(std::ostream& stream, const Vec4& vec)
  {
    return stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
  }
}
